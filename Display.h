#ifndef ROW_DISPLAY_H_
#define ROW_DISPLAY_H_

#include "Arduino.h"
#include "SPI.h"

/**
 * Display consist of a matrix of 8x8-LED-Kits. For example putting 8x5 kits together will build
 * a display with (8*8)x(8*5) pixels -> (64x40).
 *
 * Each LED represents single pixel, and it's placed on x,y position. Left top corner has (0,0) and bottom right
 * (max width-1, max height-1) - for out example it's (63,39).
 *
 * Each 8x8-LED-Kit is connected to MAX7219. All chips share common MOSI and SCK lines, MISO is not used.
 * Each chip requires separate Slave Select line.
 */
class Display {
public:
	Display(uint8_t xKits, uint8_t yKits, uint8_t **ss);

	/**
	 * Prints given matrix on the display starting at x,y position.
	 * Matrix dimension is given by #width and #height. It can exceed display size, in this case it will get trimmed.
	 * Matrix data is stored in two dimensional array of bytes, where single bit represents one pixel.
	 *
	 * For example, matrix consisting of 3x2 bytes has maximal dimension 24(3*8) on 16(2*8) pixels,
	 * which gives us a 384 pixels in total.
	 */
	void print(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t **pixels);

	/** Initialized SPI and 8x8-Matrix elements. */
	void setup();

private:
	/** Slave Select lines reflecting positions of 8x8-Matrices on the Displays */
	uint8_t **ss;

	/** Horizontal amount of 8x8-Matrices */
	uint8_t xKits;

	/** Vertical amount of 8x8-Matrices */
	uint8_t yKits;

	void setupMax(uint8_t ss);
	void send(uint8_t ss, uint8_t address, uint8_t value);
	void clear(uint8_t ss);
};

#endif /* ROW_DISPLAY_H_ */
