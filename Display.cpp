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

	// assign varargs to #ss
	this->ss = ss;
}

void Display::setup() {
	SPI.begin();
	SPI.setDataMode(SPI_MODE3);
	SPI.setClockDivider(SPI_CLOCK_DIV128);

	for (uint8_t ssX = 0; ssX < xKits; ssX++) {
		for (uint8_t ssY = 0; ssY < xKits; ssY++) {
			setupMax(ss[ssX][ssY]);
		}
	}
}

void Display::setupMax(uint8_t ss) {
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
