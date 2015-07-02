#include "Display.h"

// max7219 registers
#define REG_NOOP         0x0
#define REG_DIGIT0       0x1
#define REG_DIGIT1       0x2
#define REG_DIGIT2       0x3
#define REG_DIGIT3       0x4
#define REG_DIGIT4       0x5
#define REG_DIGIT5       0x6
#define REG_DIGIT6       0x7
#define REG_DIGIT7       0x8
#define REG_DECODEMODE   0x9
#define REG_INTENSITY    0xA
#define REG_SCANLIMIT    0xB
#define REG_SHUTDOWN     0xC
#define REG_DISPLAYTEST  0xF

Display::Display(uint8_t xKits, uint8_t yKits, uint8_t **ss) {
	this->xKits = xKits;
	this->yKits = yKits;
	this->ss = ss;

	// initialize screen buffer
	this->sbuf = new uint8_t*[xKits];
	for (int x = 0; x < xKits; x++) {
		this->sbuf[x] = new uint8_t[yKits];
		memset(this->sbuf[x], 0x00, sizeof(this->sbuf[x]));
	}

	// init screen buffer
	uint8_t rows = yKits * KIT_DIM;
	screen = new uint8_t*[rows];
	for (uint8_t y = 0; y < rows; y++) {
		screen[y] = new uint8_t[xKits];
		for (uint8_t x = 0; x < xKits; x++) {
			screen[y][x] = 0x0;
		}
	}
#if DEBUG
	debug("Created display with %dx%d LED-Kits", xKits, yKits);
#endif
}

void Display::setup() {
	setupSpi();
	setupMax();
}

void Display::setupSpi() {
	SPI.begin();
	SPI.setDataMode(SPI_MODE3);
	SPI.setClockDivider(SPI_CLOCK_DIV128);
}

void Display::setupMax() {
	for (uint8_t ssY = 0; ssY < yKits; ssY++) {
		for (uint8_t ssX = 0; ssX < xKits; ssX++) {
			setupMax(ss[ssX][ssY]);
		}
	}
}

inline uint8_t Display::limitSize(uint8_t xy, uint8_t wh, uint8_t startKitXY, uint8_t endKitXY) {
	return min(wh, (endKitXY - startKitXY + 1) * KIT_DIM - xy % KIT_DIM);
}

inline uint8_t Display::calcEndKit(uint8_t xy, uint8_t wh, uint8_t yxKits) {
	return min(((xy + wh - 1) / KIT_DIM), yxKits - 1);
}

inline uint8_t Display::calcSizeOnKit(uint8_t xy, uint8_t wh, uint8_t xyKit, uint8_t xyOnKit, uint8_t startKitXY,
		uint8_t endKitXY) {
	uint8_t widthOnKit = 0;
	if (xyKit == startKitXY) {
		widthOnKit = min(wh, KIT_DIM - xyOnKit);

	} else if (xyKit == endKitXY) {
		widthOnKit = (xy + wh) % KIT_DIM;
	}
	// happens for xKit == endKitX and all pixels enabled (data byte = 0xFF) - in this case % gives 0
	if (widthOnKit == 0) {
		widthOnKit = KIT_DIM;
	}

	return widthOnKit;
}

void Display::paint(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t **data) {
#if DEBUG
	debug("Print pixels: p[%d,%d] -> %dx%d", x, y, width, height);
#endif

	// find starting 8x8-Matrix, begins with 0
	uint8_t startKitX = x / KIT_DIM;
	uint8_t startKitY = y / KIT_DIM;

	uint8_t endKitX = calcEndKit(x, width, xKits);
	uint8_t endKitY = calcEndKit(y, height, yKits);

	uint8_t widthLim = limitSize(x, width, startKitX, endKitX);
	uint8_t heightLim = limitSize(y, height, startKitY, endKitY);

#if DEBUG
	debug("Using kits: k[%d,%d] - k[%d,%d] with pixel w/h: %dx%d", startKitX, startKitY, endKitX, endKitY, widthLim,
			heightLim);
#endif

	KitData *kd = (KitData*) malloc(sizeof(KitData));
	kd->xRelKit = 0;
	kd->yRelKit = 0;
	kd->xRelKitSize = endKitX - startKitX + 1;
	kd->yRelKitSize = endKitY - startKitY + 1;

	kd->xDataBytes = widthLim / KIT_DIM;
	if (kd->xDataBytes * KIT_DIM < widthLim) {
		kd->xDataBytes++;
	}

	for (kd->yKit = startKitY; kd->yKit <= endKitY; kd->yKit++) {
		kd->yOnScreenIdx = kd->yKit * KIT_DIM;
		if (kd->yKit == startKitY) {
			kd->yOnFirstKit = kd->yOnKit = y - (startKitY * KIT_DIM);
			kd->yOnScreenIdx += kd->yOnKit;
		} else {
			kd->yOnKit = 0;
		}

		kd->yOnKitSize = calcSizeOnKit(y, heightLim, kd->yKit, kd->yOnKit, startKitY, endKitY);

		kd->yDataIdx = kd->yRelKit == 0 ? 0 : (KIT_DIM - kd->yOnFirstKit);
		if (kd->yRelKit >= 2) {
			kd->yDataIdx += (KIT_DIM * (kd->yRelKit - 1));
		}

		kd->xRelKit = 0;
		for (kd->xKit = startKitX; kd->xKit <= endKitX; kd->xKit++) {
			if (kd->xKit == startKitX) {
				kd->xOnFirstKit = kd->xOnKit = x - (startKitX * KIT_DIM);
			} else {
				kd->xOnKit = 0;
			}
			kd->xOnKitSize = calcSizeOnKit(x, widthLim, kd->xKit, kd->xOnKit, startKitX, endKitX);
			kd->xOnScreenIdx = kd->xKit;

			paintOnKit(*kd, data); // pass kd by reference, it will be modified inside

			kd->xRelKit++;
		}
		kd->yRelKit++;
	}
	free(kd);
}

inline void Display::paintOnKit(KitData kd, uint8_t **data) {
	uint8_t ssKit = ss[kd.xKit][kd.yKit];
#if DEBUG
	debug("Print on kit(%d): k[%d,%d], kr[%d,%d] -> %dx%d, p[%d,%d] -> %dx%d", ssKit, kd.xKit, kd.yKit, kd.xRelKit,
			kd.yRelKit, kd.xRelKitSize, kd.yRelKitSize, kd.xOnKit, kd.yOnKit, kd.xOnKitSize, kd.yOnKitSize);
#endif

	// go over rows on single LED-Kit
	for (uint8_t yOnKit = 0; yOnKit < kd.yOnKitSize; yOnKit++) {
		if (kd.xOnFirstKit == 0) {
			paintRowOverlaps(&kd, data);
		} else {
			if (kd.xRelKit == 0) {
				paintRowOnFirstKit(&kd, data);
			} else {
				if (kd.xRelKit < kd.xDataBytes) {
					paintRow2Bytes(&kd, data);
				} else {
					paintRow1ByteOnLastKit(&kd, data);
				}
			}
		}
		kd.yOnKit++;
		kd.yDataIdx++;
	}
}

inline void Display::paintRowOverlaps(KitData *kd, uint8_t **data) {
	uint8_t yByte = data[kd->yDataIdx][kd->xKit];

#if DEBUG
	debug("-- Overlap (%d) -> data[%d][%d] = 0x%02x", kd->yOnKit, kd->yDataIdx, kd->xKit, yByte);
#endif

	// TODO
}

inline void Display::paintRowOnFirstKit(KitData *kd, uint8_t **data) {
	uint8_t yByte = data[kd->yDataIdx][0];
	uint8_t dispByte = screen[kd->yOnScreenIdx][kd->xOnScreenIdx];
	uint8_t newDispByte = (dispByte & maskL(kd->xOnKit)) | (yByte & maskR(KIT_DIM - kd->xOnKit));
	screen[kd->yOnScreenIdx][kd->xOnScreenIdx] = newDispByte;

#if DEBUG
	String fnewDispByte;
	fbyte(newDispByte, fnewDispByte);
	debug("-- First Kit (%d) -> data[%d][0] = 0x%02x, screen[%d][%d] = 0x%02x <- %s", kd->yOnKit, kd->yDataIdx, yByte,
			kd->yOnScreenIdx, kd->xOnScreenIdx, dispByte, newDispByte);
#endif
}

inline void Display::paintRow2Bytes(KitData *kd, uint8_t **data) {
	uint8_t yByte = data[kd->yDataIdx][kd->xRelKit - 1];
	uint8_t yByteRight = data[kd->yDataIdx][kd->xRelKit];
	uint8_t newDispByte =

#if DEBUG
	debug("-- Kit 2 byte (%d) -> data[%d][%d] = 0x%02x, data[%d][%d] = 0x%02x", kd->yOnKit, kd->yDataIdx,
			kd->xRelKit - 1, yByte, kd->yDataIdx, kd->xRelKit, yByteRight);
#endif

	// TODO
}

inline void Display::paintRow1ByteOnLastKit(KitData *kd, uint8_t **data) {
	uint8_t yByte = data[kd->yDataIdx][kd->xRelKit - 1];
#if DEBUG
	debug("-- Kit 1 byte (%d) -> data[%d][%d] = 0x%02x", kd->yOnKit, kd->yDataIdx, kd->xRelKit - 1, yByte);
#endif

	// TODO
}

void Display::setupMax(uint8_t ss) {
#if DEBUG
	debug("Configuring MAX7219 on SS: %d", ss);
#endif
	/*
	 pinMode(ss, OUTPUT);

	 // disable shutdown mode
	 send(ss, REG_SHUTDOWN, 1);

	 // display test
	 send(ss, REG_DISPLAYTEST, 0);

	 send(ss, REG_INTENSITY, 15);  // character intensity: range: 0 to 15
	 send(ss, REG_SCANLIMIT, 7);   // show all 8 digits
	 send(ss, REG_DECODEMODE, 0);  // using an led matrix (not digits)
	 */
}

void Display::clearKit(uint8_t ss) {
	send(ss, REG_DIGIT0, 0);
	send(ss, REG_DIGIT1, 0);
	send(ss, REG_DIGIT2, 0);
	send(ss, REG_DIGIT3, 0);
	send(ss, REG_DIGIT4, 0);
	send(ss, REG_DIGIT5, 0);
	send(ss, REG_DIGIT6, 0);
	send(ss, REG_DIGIT7, 0);
}

/* Transfers data to a MAX7219. */
void Display::send(uint8_t ss, uint8_t address, uint8_t data) {
	/*
	 #if DEBUG
	 debug("Send(%d): %d = 0x%02x", ss, address, data);
	 #endif

	 // Ensure LOAD/CS is LOW
	 digitalWrite(ss, LOW);

	 // Send the register address
	 SPI.transfer(address);

	 // Send the value
	 SPI.transfer(data);

	 // Tell chip to load in data
	 digitalWrite(ss, HIGH);
	 */
}
