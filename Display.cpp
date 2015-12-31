#include "Display.h"

Display::Display(kit_t xKits, kit_t yKits, ss_t **ss) :
		xKits(xKits), yKits(yKits), rows(yKits * KIT_DIM), ss(ss), screen(init2DArray8(rows, xKits)) {

#if LOG_DI
	log(F("Created display with %dx%d LED-Kits and %d bytes screen buffer"), xKits, yKits, (rows * xKits));
#endif
}

Display::~Display() {
	delete2DArray8(screen);
}

void Display::flush() {
#if LOG_DI
	log(F("Flushing display: %dx%d"), yKits, xKits);
#endif
	for (kit_t yKit = 0; yKit < yKits; yKit++) {
		uint8_t displayRow = yKit * KIT_DIM;
		ss_t *ssXRf = ss[yKit];
		for (uint8_t kitRow = REG_DIGIT0; kitRow <= REG_DIGIT7; kitRow++) {
			uint8_t *screenXRef = screen[displayRow];
			for (kit_t xKit = 0; xKit < xKits; xKit++) {
				ss_t ssAddress = ssXRf[xKit];
				uint8_t screenByte = screenXRef[xKit];
#if LOG_DI
				log(F("Line: %dx%d - %d ss:%d"), yKit, xKit, kitRow, ssAddress);
#endif
				send(ssAddress, kitRow, screenByte);
			}
			displayRow++;
		}
	}

}

void Display::setup() {
	setupSpi();
	setupMax();
	clear();
	flush();
}

void Display::clear() {
	clean2DArray8(screen, rows, xKits);
}

void Display::setupSpi() {
#if SIMULATE_DI
	return;
#endif
	SPI.begin();
	SPI.setDataMode(SPI_MODE3);
	SPI.setClockDivider(SPI_CLOCK_DIV128);
}

void Display::setupMax() {
#if SIMULATE_DI
	return;
#endif
	for (uint8_t ssY = 0; ssY < yKits; ssY++) {
		for (uint8_t ssX = 0; ssX < xKits; ssX++) {
			setupMax(ss[ssY][ssX]);
		}
	}
}

inline pixel_t Display::limitSize(pixel_t xy, pixel_t wh, kit_t startKitXY, kit_t endKitXY) {
	return min(wh, (endKitXY - startKitXY + 1) * KIT_DIM - xy % KIT_DIM);
}

inline kit_t Display::calcEndKit(pixel_t xy, pixel_t wh, kit_t yxKits) {
	return min(((xy + wh - 1) / KIT_DIM), yxKits - 1);
}

inline pixel_t Display::calcSizeOnKit(pixel_t xy, pixel_t wh, kit_t xyKit, kit_t xyOnKit, kit_t startKitXY,
		kit_t endKitXY) {
	uint8_t widthOnKit = 0;
	if (xyKit == startKitXY) { // first kit
		widthOnKit = min(wh, KIT_DIM - xyOnKit);

	} else if (xyKit == endKitXY) { // last kit
		widthOnKit = (xy + wh) % KIT_DIM;
	}
	// happens for xKit == endKitX and all pixels enabled (data byte = 0xFF) - in this case % gives 0
	if (widthOnKit == 0) {
		widthOnKit = KIT_DIM;
	}

	return widthOnKit;
}

void Display::clear(pixel_t x, pixel_t y, pixel_t width, pixel_t height) {
#if LOG_DI
	log(F("Clear display: p(%d,%d) -> %dx%d"), x, y, width, height);
#endif

	paint(x, y, width, height, NULL);
}

void Display::paint(pixel_t x, pixel_t y, pixel_t width, pixel_t height, uint8_t **data) {
#if LOG_DI
	log(F("Paint pixels: p(%d,%d) -> %dx%d"), x, y, width, height);
#endif

	// find starting 8x8-Matrix, begins with 0
	kit_t startKitX = x / KIT_DIM;
	kit_t startKitY = y / KIT_DIM;

	kit_t endKitX = calcEndKit(x, width, xKits);
	kit_t endKitY = calcEndKit(y, height, yKits);

	pixel_t widthLim = limitSize(x, width, startKitX, endKitX);
	pixel_t heightLim = limitSize(y, height, startKitY, endKitY);

#if LOG_DI
	log(F("Using kits: k(%d,%d) - k(%d,%d) with pixel w/h: %dx%d"), startKitX, startKitY, endKitX, endKitY, widthLim,
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

			paintOnKit(*kd, data); // pass kd by value, it will be modified inside

			kd->xRelKit++;
		}
		kd->yRelKit++;
	}
	free(kd);
}

inline void Display::paintOnKit(KitData kd, uint8_t **data) {
#if LOG_DI
	log(F("Paint on kit: k(%d,%d), kr(%d,%d) -> %dx%d, p(%d,%d) -> %dx%d"), kd.xKit, kd.yKit, kd.xRelKit, kd.yRelKit,
			kd.xRelKitSize, kd.yRelKitSize, kd.xOnKit, kd.yOnKit, kd.xOnKitSize, kd.yOnKitSize);
#endif

	// go over rows on single LED-Kit
	for (pixel_t yCnt = 0; yCnt < kd.yOnKitSize; yCnt++) {
		uint8_t newDispByte;

		if (kd.xOnFirstKit == 0) { // x position on first kit is not shifted
			if (kd.xRelKit == kd.xRelKitSize - 1) {
				newDispByte = overlapped_lastKit(&kd, data);
			} else {
				newDispByte = overlapped_firstAndMidleKit(&kd, data);
			}
		} else {

			if (kd.xRelKit == 0) { // paint on first kit
				newDispByte = shifted_firstKit(&kd, data);

			} else if (kd.xRelKit == kd.xRelKitSize - 1) { // paint on last kit
				if (kd.xRelKit < kd.xDataBytes) {
					newDispByte = shifted_lastKit2Bytes(&kd, data);

				} else {
					newDispByte = shifted_lastKit1Byte(&kd, data);
				}
			} else { // paint on middle kit
				newDispByte = shifted_middleKit(&kd, data);
			}
		}
		screen[kd.yOnScreenIdx][kd.xOnScreenIdx] = newDispByte;
		kd.yDataIdx++;
		kd.yOnScreenIdx++;
	}
}

inline uint8_t Display::overlapped_firstAndMidleKit(KitData *kd, uint8_t **data) {
	uint8_t newDispByte = data == NULL ? 0 : data[kd->yDataIdx][kd->xRelKit];

#if LOG_DI
	char fnewDispByte[9];
	fbyte(newDispByte, fnewDispByte);

	log(F("-- overlapped_firstAndMidleKit (%d,%d) -> screen[%d][%d] = %s"), kd->xOnKit, kd->yOnKit, kd->yOnScreenIdx,
			kd->xOnScreenIdx, fnewDispByte);
#endif

	return newDispByte;
}

inline uint8_t Display::overlapped_lastKit(KitData *kd, uint8_t **data) {
	uint8_t yByte = data == NULL ? 0 : data[kd->yDataIdx][kd->xRelKit];
	uint8_t yByteMasked = yByte & maskL(kd->xOnKitSize);

	uint8_t screenByte = screen[kd->yOnScreenIdx][kd->xOnScreenIdx];
	uint8_t screenByteMasked = screenByte & maskR(KIT_DIM - kd->xOnKitSize);

	uint8_t newDispByte = yByteMasked | screenByteMasked;

#if LOG_DI
	char fnewDispByte[9];
	fbyte(newDispByte, fnewDispByte);

	char fyByte[9];
	fbyte(yByte, fyByte);

	log(F("-- overlapped_lastKit (%d,%d) -> data[%d][%d] = %s, screen[%d][%d] = %s"), kd->xOnKit, kd->yOnKit,
			kd->yDataIdx, kd->xRelKit, fyByte, kd->yOnScreenIdx, kd->xOnScreenIdx, fnewDispByte);
#endif

	return newDispByte;
}

inline uint8_t Display::shifted_firstKit(KitData *kd, uint8_t **data) {
	uint8_t yByte = data == NULL ? 0 : data[kd->yDataIdx][0];
	uint8_t yByteMasked = yByte >> kd->xOnKit;

	uint8_t screenByte = screen[kd->yOnScreenIdx][kd->xOnScreenIdx];
	uint8_t screenByteMasked = screenByte & maskL(kd->xOnKit);

	uint8_t newDispByte = screenByteMasked | yByteMasked;

#if LOG_DI
	char fnewDispByte[9];
	fbyte(newDispByte, fnewDispByte);

	char fyByte[9];
	fbyte(yByte, fyByte);

	log(F("-- shifted_firstKit (%d,%d) -> data[%d][0] = %s, screen[%d][%d] = %s"), kd->xOnKit, kd->yOnKit, kd->yDataIdx,
			fyByte, kd->yOnScreenIdx, kd->xOnScreenIdx, fnewDispByte);
#endif

	return newDispByte;
}

inline uint8_t Display::shifted_middleKit(KitData *kd, uint8_t **data) {
	uint8_t yByteLeft = data == NULL ? 0 : data[kd->yDataIdx][kd->xRelKit - 1];
	uint8_t yByteLeftMasked = yByteLeft << (KIT_DIM - kd->xOnFirstKit);

	uint8_t yByteRight = data == NULL ? 0 : data[kd->yDataIdx][kd->xRelKit];
	uint8_t yByteRightMasked = yByteRight >> kd->xOnFirstKit;

	uint8_t newDispByte = yByteLeftMasked | yByteRightMasked;

#if LOG_DI
	char fnewDispByte[9];
	fbyte(newDispByte, fnewDispByte);

	char fyByteLeft[9];
	fbyte(yByteLeft, fyByteLeft);

	char fyByteRight[9];
	fbyte(yByteRight, fyByteRight);

	log(F("-- shifted_middleKit (%d,%d) -> l-data[%d][%d] = %s, r-data[%d][%d] = %s, screen[%d][%d] = %s"), kd->xOnKit,
			kd->yOnKit, kd->yDataIdx, kd->xRelKit - 1, fyByteLeft, kd->yDataIdx, kd->xRelKit, fyByteRight,
			kd->yOnScreenIdx, kd->xOnScreenIdx, fnewDispByte);
#endif

	return newDispByte;
}

inline uint8_t Display::shifted_lastKit2Bytes(KitData *kd, uint8_t **data) {
	uint8_t yByteLeft = data == NULL ? 0 : data[kd->yDataIdx][kd->xRelKit - 1];
	uint8_t yByteLeftMasked = yByteLeft << (KIT_DIM - kd->xOnFirstKit);

	uint8_t yByteRight = data == NULL ? 0 : data[kd->yDataIdx][kd->xRelKit];
	uint8_t yByteRightMasked = (yByteRight >> kd->xOnFirstKit) & maskL(kd->xOnKitSize);

	uint8_t screenByte = screen[kd->yOnScreenIdx][kd->xOnScreenIdx];
	uint8_t screenByteMasked = screenByte & maskR(KIT_DIM - kd->xOnKitSize);

	uint8_t newDispByte = yByteLeftMasked | yByteRightMasked | screenByteMasked;

#if LOG_DI
	char fnewDispByte[9];
	fbyte(newDispByte, fnewDispByte);

	char fyByteLeft[9];
	fbyte(yByteLeft, fyByteLeft);

	char fyByteRight[9];
	fbyte(yByteRight, fyByteRight);

	log(F("-- shifted_lastKit2Bytes (%d,%d) -> l-data[%d][%d] = %s, r-data[%d][%d] = %s, screen[%d][%d] = %s"),
			kd->xOnKit, kd->yOnKit, kd->yDataIdx, kd->xRelKit - 1, fyByteLeft, kd->yDataIdx, kd->xRelKit, fyByteRight,
			kd->yOnScreenIdx, kd->xOnScreenIdx, fnewDispByte);
#endif

	return newDispByte;
}

inline uint8_t Display::shifted_lastKit1Byte(KitData *kd, uint8_t **data) {
	uint8_t screenBits = KIT_DIM - kd->xOnFirstKit;
	uint8_t yByte = data == NULL ? 0 : data[kd->yDataIdx][kd->xRelKit - 1];
	uint8_t yByteMasked = yByte << screenBits;

	uint8_t screenByte = screen[kd->yOnScreenIdx][kd->xOnScreenIdx];
	uint8_t screenByteMasked = screenByte & maskR(screenBits);

	uint8_t newDispByte = screenByteMasked | yByteMasked;

#if LOG_DI
	char fnewDispByte[9];
	fbyte(newDispByte, fnewDispByte);

	char fyByte[9];
	fbyte(data[kd->yDataIdx][0], fyByte);

	log(F("-- shifted_lastKit1Byte (%d,%d) -> data[%d][0] = %s, screen[%d][%d] = %s"), kd->xOnKit, kd->yOnKit,
			kd->yDataIdx, fyByte, kd->yOnScreenIdx, kd->xOnScreenIdx, fnewDispByte);
#endif

	return newDispByte;
}

void Display::setupMax(ss_t ss) {
#if LOG_DI
	log(F("Configuring MAX7219 on SS: %d"), ss);
#endif

	pinMode(ss, OUTPUT);

	// disable shutdown mode
	send(ss, REG_SHUTDOWN, 1);

	// display test
	send(ss, REG_DISPLAYTEST, 0);

	send(ss, REG_INTENSITY, 15);  // character intensity: range: 0 to 15
	send(ss, REG_SCANLIMIT, 7);   // show all 8 digits
	send(ss, REG_DECODEMODE, 0);  // using an led matrix (not digits)
	clearKit(ss);
}

void Display::clearKit(ss_t ss) {
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
void Display::send(ss_t ss, uint8_t address, uint8_t data) {
#if SIMULATE_DI
	return;
#endif
#if LOG_DI
	log(F("Send(%d): %d = 0x%02x"), ss, address, data);
#endif

	// Ensure LOAD/CS is LOW
	digitalWrite(ss, LOW);

	// Send the register address
	SPI.transfer(address);

	// Send the value
	SPI.transfer(data);

	// Tell chip to load in data
	digitalWrite(ss, HIGH);

}
