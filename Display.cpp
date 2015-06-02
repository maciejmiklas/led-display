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

void Display::print(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t **pixels) {
#if DEBUG
	debug("Print pixels: p[%d,%d] -> %dx%d", x, y, width, height);
#endif

	// find starting 8x8-Matrix, begins with 0
	uint8_t startKitX = x / M_DIM;
	uint8_t startKitY = y / M_DIM;

	// find ending 8x8-Matrix - exclusive
	uint8_t endKitX = min(((x + width - 1) / M_DIM), xKits - 1);
	uint8_t endKitY = min(((y + height - 1) / M_DIM), yKits - 1);

	// reduce width/height so it fits on the screen
	width = min(width, (endKitX - startKitX + 1) * M_DIM - x % M_DIM);
	height = min(height, (endKitY - startKitY + 1) * M_DIM - y % M_DIM);

#if DEBUG
	debug("Using kits: k[%d,%d] - k[%d,%d] with pixel w/h: %dx%d", startKitX, startKitY, endKitX, endKitY, width, height);
#endif

	for (uint8_t yKit = startKitY; yKit <= endKitY; yKit++) {
		for (uint8_t xKit = startKitX; xKit <= endKitX; xKit++) {

			// calculate starting (x,y) position within current kit
			uint8_t xOnKit;
			uint8_t yOnKit;
			if (xKit == startKitX && yKit == startKitY) {
				xOnKit = x - (startKitX * M_DIM);
				yOnKit = y - (startKitY * M_DIM);
			} else {
				xOnKit = 0;
				yOnKit = 0;
			}

			// calculate width within current kit
			uint8_t widthOnKit = 0;
			if (xKit == startKitX) {
				widthOnKit = min(width, M_DIM - xOnKit);
			} else if (xKit == endKitX) {
				widthOnKit = (x + width) % M_DIM;
			}
			// happens for xKit == endKitX and all pixels on (0xFF) - in this case module gives 0
			if (widthOnKit == 0) {
				widthOnKit = M_DIM;
			}

			// calculate height within current kit
			uint8_t heightOnKit = 0;
			if (yKit == startKitY) {
				heightOnKit = min(height, M_DIM - yOnKit);
			} else if (yKit == endKitY) {
				heightOnKit = (y + height) % M_DIM;
			}
			if (heightOnKit == 0) {
				heightOnKit = M_DIM;
			}

#if DEBUG
			debug("Print on kit: k[%d,%d], p[%d,%d] -> %dx%d", xKit, yKit, xOnKit, yOnKit, widthOnKit, heightOnKit);
#endif
		}
	}
}

void printlnOnKit(uint8_t x, uint8_t y, uint8_t width, uint8_t line) {
#if DEBUG
	debug("Print on Kit [%d,%d], with: %d, line: 0x%02x", x, y, width, line);
#endif
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
