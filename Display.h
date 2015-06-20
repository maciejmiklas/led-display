#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "Arduino.h"
#include "SPI.h"
#include "Log.h"

#define DEBUG true

/**
 * Dimension in pixels (LEDs) of LED-Matrix-Kit.
 * Currently #KIT_DIM is limited to 8, because pixel data is stored in byte array and we are assuming,
 * that one byte call fully cover one row on single LED-Kit.
 */
#define KIT_DIM 8

/**
 * Display consist of a matrix of 8x8-LED-Kits. For example putting 8x5 kits together will build
 * a display with (8*8)x(8*5) pixels -> (64x40).
 *
 * Each LED represents single pixel, and it's placed on (x,y) position. Left top corner has (0,0) and bottom right
 * (max width-1, max height-1) - in our example above it's (63,39).
 *
 * Each 8x8-LED-Kit is connected to MAX7219. All chips share common MOSI and SCK lines, MISO is not used.
 * Each chip requires separate Slave Select line.
 *
 * Display orientation:
 * (0,0) -----------------------------> [x]
 *      |
 *      |
 *      |
 *      |
 *      |
 *      |
 *      |                            (x max, y max)
 *      v [y]
 *
 * Pixels on Display consisting of 4x5 LED-Kits
 *
 0        1        2       3
 0 ........|........|........|........   (8,0)|(15,0)|(23,0)|(31,0)
 1 ........|........|........|........   (8,1)|(15,1)|(23,1)|(31,1)
 2 ........|........|........|........   (8,2)|(15,2)|(23,2)|(31,2)
 3 ........|........|........|........   (8,3)|(15,3)|(23,3)|(31,3)
 4 ........|........|........|........   (8,4)|(15,4)|(23,4)|(31,4)

 */
class Display {
public:

	/**
	 * #ss contains Select Slave lines for 8x8-LED-Kits, The first coordinate in table points horizontal
	 * position, second vertical: ss[horizontal][vertical].
	 */
	Display(uint8_t xKits, uint8_t yKits, uint8_t **ss);

	/**
	 * Prints given matrix on the display starting at pixel on (x,y) position.
	 * Matrix dimension in pixels is given by #width and #height. It can exceed display size, in this case it will
	 * get trimmed. Matrix data is stored in two dimensional array of bytes, where single bit represents one pixel,
	 * first position in array indicates x second y: pixels[x][y].
	 *
	 * For example, matrix consisting of 3x2 bytes has maximal dimension 24(3*8) on 16(2*8) pixels,
	 * which gives us a 384 pixels in total.
	 */
	void print(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t **pixels);

	/** Initialized SPI and 8x8-Matrix elements. */
	void setup();

private:
	/**
	 * Slave Select lines reflecting positions of 8x8-Matrices on the Displays.
	 * It has the same orientation as #sbuf
	 */
	uint8_t **ss;

	/**
	 * Screen buffer containing current content of the screen. Orientation: sbuf[xKits][yKits] -
	 * first dimension represents column (x), second row within particular column (y) - like in The Marix ;)
	 */
	uint8_t **sbuf;

	/** Horizontal amount of 8x8-Matrices, known as Kits */
	uint8_t xKits;

	/** Vertical amount of 8x8-Matrices */
	uint8_t yKits;

	typedef struct {
		// all (x,y) coordinates starting from 0 and are inclusive.

		// starting pixels on first LED-Kit
		uint8_t xOnFirstKit;
		uint8_t yOnFirstKit;

		// (x,y) position starting from first Kit that we are panting on.
		uint8_t xRelKit;
		uint8_t yRelKit;

		// (x,y) Kit position starting from first Kit in LED-Kit-Matrix.
		int8_t xKit;
		uint8_t yKit;

		// (x,y) position on kit that we are painting on.
		uint8_t xOnKit;
		uint8_t yOnKit;

		// dimensions on kit that we are painting on.
		uint8_t widthOnKit;
		uint8_t heightOnKit;
	} KitData;

	void setupMax();
	void setupMax(uint8_t ss);
	void setupSpi();
	void send(uint8_t ss, uint8_t address, uint8_t value);
	void clear(uint8_t ss);
	inline uint8_t limitSize(uint8_t xy, uint8_t wh, uint8_t startKitXY, uint8_t endKitXY);
	inline uint8_t calcEndKit(uint8_t xy, uint8_t wh, uint8_t yxKits);
	inline uint8_t calcSizeOnKit(uint8_t xy, uint8_t wh, uint8_t xyKit, uint8_t xyOnKit, uint8_t startKitXY,
			uint8_t endKitXY);
	inline void printOnKit(KitData *kd, uint8_t **data);
	inline boolean isEageKitOnX(KitData *kd);
};

#endif /* DISPLAY_H_ */
