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
	debug("Print (%d,%d) with [%dx%d]", x, y, width, height);
#endif

	// find starting 8x8-Matrix
	uint8_t sKitX = x / M_DIM;
	uint8_t sKitY = y / M_DIM;

	for (uint8_t yKit = sKitY; yKit < yKits; yKit++) {
		for (uint8_t xKit = sKitX; xKit < xKits; xKit++) {

			// calculate starting x,y position within current kit
			uint8_t xOnKit = 0;
			uint8_t yOnKit = 0;
			if (xKit == sKitX && yKit == sKitY) {
				xOnKit = x - (sKitX * M_DIM);
				yOnKit = y - (sKitY * M_DIM);
#if DEBUG
				debug("Starting pixel (%d,%d) on kit (%d,%d)", xOnKit, yOnKit, sKitX, sKitY);
#endif
			}
		}
	}
}

void printKit(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t *pixels) {
#if DEBUG
	debug("Print on Kit (%d,%d) with [%dx%d]", x, y, width, height);
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
