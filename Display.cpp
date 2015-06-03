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

/** reduce width/height so it fits on the screen */
inline uint8_t Display::calcSize(uint8_t xy, uint8_t wh, uint8_t startKitXY, uint8_t endKitXY) {
	return min(wh, (endKitXY - startKitXY + 1) * M_DIM - xy % M_DIM);
}

/** find ending 8x8-Matrix - inclusive */
inline uint8_t Display::calcEndKit(uint8_t xy, uint8_t wh, uint8_t yxKits) {
	return min(((xy + wh - 1) / M_DIM), yxKits - 1);
}

/** calculate width/height within current kit */
inline uint8_t Display::calcSizeOnKit(uint8_t xy, uint8_t wh, uint8_t xyKit, uint8_t xyOnKit, uint8_t startKitXY,
		uint8_t endKitXY) {
	uint8_t widthOnKit = 0;
	if (xyKit == startKitXY) {
		widthOnKit = min(wh, M_DIM - xyOnKit);
	} else if (xyKit == endKitXY) {
		widthOnKit = (xy + wh) % M_DIM;
	}
	// happens for xKit == endKitX and all pixels on (0xFF) - in this case % gives 0
	if (widthOnKit == 0) {
		widthOnKit = M_DIM;
	}

	return widthOnKit;
}

void Display::print(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t **pixels) {
#if DEBUG
	debug("Print pixels: p[%d,%d] -> %dx%d", x, y, width, height);
#endif

	// find starting 8x8-Matrix, begins with 0
	uint8_t startKitX = x / M_DIM;
	uint8_t startKitY = y / M_DIM;

	uint8_t endKitX = calcEndKit(x, width, xKits);
	uint8_t endKitY = calcEndKit(y, height, yKits);

	width = calcSize(x, width, startKitX, endKitX);
	height = calcSize(y, height, startKitY, endKitY);

#if DEBUG
	debug("Using kits: k[%d,%d] - k[%d,%d] with pixel w/h: %dx%d", startKitX, startKitY, endKitX, endKitY, width,
			height);
#endif

	uint8_t xPixels = 0;
	uint8_t yPixels = 0;
	for (uint8_t yKit = startKitY; yKit <= endKitY; yKit++) {
		uint8_t yOnKit = yKit == startKitY ? y - (startKitY * M_DIM) : 0;
		uint8_t heightOnKit = calcSizeOnKit(y, height, yKit, yOnKit, startKitY, endKitY);
		//TODO use heightOnKit to limit height
		xPixels = 0;
		for (uint8_t xKit = startKitX; xKit <= endKitX; xKit++) {
			uint8_t xOnKit = xKit == startKitX ? x - (startKitX * M_DIM) : 0;
			uint8_t widthOnKit = calcSizeOnKit(x, width, xKit, xOnKit, startKitX, endKitX);
			uint8_t data = pixels[xPixels][yPixels];
			uint8_t ssKit = ss[xKit][yKit];
#if DEBUG
			debug("Print on kit: k[%d,%d], p[%d,%d] -> %dx%d, SS: %d, data: 0x%02x ", xKit, yKit, xOnKit, yOnKit,
					widthOnKit, heightOnKit, ssKit, data);
#endif
			//printlnOnKit(xOnKit, yOnKit, widthOnKit, ssKit, data);
			xPixels++;
		}
		yPixels++;
	}
}

inline void printlnOnKit(uint8_t x, uint8_t y, uint8_t width, uint8_t ssKit, uint8_t data) {

}

void Display::setupMax(uint8_t ss) {
#if DEBUG
	debug("Configuring MAX7219 on SS: %d", ss);
#endif

	pinMode(ss, OUTPUT);

	// disable shutdown mode
	send(ss, REG_SHUTDOWN, 1);

	// display test
	send(ss, REG_DISPLAYTEST, 0);

	send(ss, REG_INTENSITY, 15);  // character intensity: range: 0 to 15
	send(ss, REG_SCANLIMIT, 7);   // show all 8 digits
	send(ss, REG_DECODEMODE, 0);  // using an led matrix (not digits)
}

void Display::clear(uint8_t ss) {
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

// Ensure LOAD/CS is LOW
	digitalWrite(ss, LOW);

// Send the register address
	SPI.transfer(address);

// Send the value
	SPI.transfer(data);

// Tell chip to load in data
	digitalWrite(ss, HIGH);
}
