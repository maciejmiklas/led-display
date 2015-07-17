#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "Arduino.h"
#include "SPI.h"
#include "Log.h"

#define DEBUG true

/**
 * Dimension in pixels (LEDs) of LED-Matrix-Kit.
 * Currently #KIT_DIM is fixed to 8, because pixel data is stored in byte array and we are assuming,
 * that one byte fully covers one row on single LED-Kit.
 */
#define KIT_DIM 8

/**
 * Display consist of a matrix of 8x8-LED-Kits. For example putting 8x5 kits together will build
 * a display with (8*8)x(8*5) pixels -> (64x40).
 *
 * Each LED represents single pixel, and it's placed on (x,y) position. Left top corner has (0,0) and bottom right
 * (max width-1, max height-1) - in our example above it's (63,39).
 *
 * Each 8x8-LED-Kit is driven by MAX7219. All chips share common MOSI and SCK lines, MISO is not used.
 * Each chip requires separate Slave Select line.
 *
 * Display orientation:
 * (0,0) -----------------------------> (x)
 *      |
 *      |
 *      |
 *      |
 *      |
 *      |
 *      |                            (x max, y max)
 *      v (y)
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
	 * #ss contains Select Slave lines for 8x8-LED-Kits, The first coordinate in table indicates horizontal
	 * position, second vertical: ss[horizontal][vertical].
	 */
	Display(uint8_t xKits, uint8_t yKits, uint8_t **ss);

	/**
	 * Paints given matrix on the display starting at pixel on (x,y) position - this position is relative to top left
	 * corner of whole display.
	 * Matrix dimension in pixels is given by #width and #height. This gives us a following rectangle:
	 * (x,y)-(#width-1,#height-1).
	 *
	 * Painted data can exceed display size, in this case it will be trimmed.
	 *
	 * Matrix data is stored in two dimensional array of bytes (#data), where single bit represents one pixel.
	 * First position in #data array indicates row, second represents pixels within this row: #data[y][x]. Each byte
	 * contains 8 pixels. For example: structure consisting of 5 rows and 14 pixels per row has dimension: data[5][2] -
	 * we need two bytes per row to express 14 pixels. Also second byte of each row uses only first 6 bits -> 8 + 6 = 14
	 *
	 * For example, matrix consisting of 3x2 bytes has maximal dimension 16(2*8) x 3 pixels,
	 * which gives us a 48 pixels in total. We have 3 rows, each one has two bytes.
	 */
	void paint(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t **data);

	/** Initialized SPI and 8x8-Matrix elements. */
	void setup();

private:
	/**
	 * Slave Select lines reflecting positions of 8x8-Matrices on the Displays.
	 * It has the same orientation as #sbuf
	 */
	uint8_t **ss;

	/** Horizontal amount of 8x8-Matrices, known as Kits */
	uint8_t xKits;

	/** Vertical amount of 8x8-Matrices */
	uint8_t yKits;

	/**
	 * It's a two dimensional array containing pixels for LED-Kits. Each LED-Kit has embedded memory, but we need to
	 * keep pixel state in RAM in order to calculate row value for case when more then one sprite has to be displayed
	 * on single LED-Kit.
	 * Print method displays single sprite that eventually crosses few LED-Kits, we have only to set pixels for this
	 * sprite without affecting it direct neighbors. We have to calculate byte (row has 8 pixels) for single row on
	 * particular kit that contains pixels for already painted sprite and from new one.
	 *
	 * First dimension in display data indicates row, second kit. For example #displayRAM[2][3] indicates
	 * 3rd row on 4th kit (starting form 0).
	 */
	uint8_t **screen;

	/**
	 * Contains data for each LED-Kit. Painting procedure goes over all kits and KitData will get recalculated
	 * for each Kit.
	 */
	typedef struct {
		// all (x,y) coordinates are starting from 0 and are inclusive.

		/** starting pixels on first LED-Kit */
		uint8_t xOnFirstKit;
		uint8_t yOnFirstKit;

		/** (x,y) position starting from first Kit that we are panting on. */
		uint8_t xRelKit;
		uint8_t yRelKit;

		/** Amount of LED-Kits used for painting */
		uint8_t xRelKitSize;
		uint8_t yRelKitSize;

		/**
		 * Amount of bytes from #data containing pixels for x-axis (vertical) - second argument of #data[y][x].
		 * Amount is already limited so that it only covers Kits that will be used for painting.
		 */
		uint8_t xDataBytes;

		/** (x,y) Kit position starting from first Kit in LED-Kit-Matrix. */
		uint8_t xKit;
		uint8_t yKit;

		/** (x,y) position on kit that we are painting on. */
		uint8_t xOnKit;
		uint8_t yOnKit;

		/** dimensions on kit that we are painting on. */
		uint8_t xOnKitSize;
		uint8_t yOnKitSize;

		/** (x,y) for #screen[y,x]  */
		uint8_t xOnScreenIdx;
		uint8_t yOnScreenIdx;

		// y position on #data
		uint8_t yDataIdx;
	} KitData;

	void setupMax();
	void setupMax(uint8_t ss);
	void setupSpi();
	void send(uint8_t ss, uint8_t address, uint8_t value);
	void clearKit(uint8_t ss);

	/** reduces width/height so it fits on the screen */
	inline uint8_t limitSize(uint8_t xy, uint8_t wh, uint8_t startKitXY, uint8_t endKitXY);

	/** finds ending 8x8-Matrix - inclusive */
	inline uint8_t calcEndKit(uint8_t xy, uint8_t wh, uint8_t yxKits);

	/** calculates width/height within current kit */
	inline uint8_t calcSizeOnKit(uint8_t xy, uint8_t wh, uint8_t xyKit, uint8_t xyOnKit, uint8_t startKitXY,
			uint8_t endKitXY);

	/** Passes kd by reference, because values will get modified inside function */
	inline void paintOnKit(KitData kd, uint8_t **data);

	// overlapped_xxx -> vertical position of data is not shifted relatively to first kit data consists of 8 bit
	// values and those align perfectly with 8 LED rows

	inline uint8_t overlapped_firstAndMidleKit(KitData *kd, uint8_t **data);
	inline uint8_t overlapped_lastKit(KitData *kd, uint8_t **data);

	// shifted_xxx -> vertical position of data IS shifted relatively to first kit

	inline uint8_t shifted_firstKit(KitData *kd, uint8_t **data);

	/**
	 *  Vertical position on first kit is shifted, so we need two bytes of data to cover single row on one LED-Kit.
	 *  This method will be used to paint row on middle kit.
	 */
	inline uint8_t shifted_middleKit(KitData *kd, uint8_t **data);

	/**
	 *  This is exception to method using 2 bytes. On the last kit it might be sufficient to use only one byte to
	 *  print single row, because width limits amount of pixel within row.
	 *
	 *  For example: we have 3 LED-Kits in vertical position, width set to 15, and y start position set to 3.
	 *  In this case we will set flowing bits:
	 *  on first kit: 3-8 -> uses 5 bits of first byte
	 *  on second kit: 0-8 -> uses 3 bits of first byte and 5 bits of second byte
	 *  on third kit: 0-1 -> uses 2 bits of second byte
	 *  On the last kit we need only single byte.
	 *
	 *  Another example: we have 3 LED-Kits in vertical position, width set to 17, and y start position set to 3.
	 *  In this case we will set flowing bits:
	 *  on first kit: 3-8 -> uses 5 bits of first byte
	 *  on second kit: 0-8 -> uses 3 bits of first byte and 5 bits of second byte
	 *  on third kit: 0-3 -> uses 3 bits of second and 1 bit of third byte
	 *  On the last kit we need 2 bytes.
	 */
	inline uint8_t shifted_lastKit1Byte(KitData *kd, uint8_t **data);

	inline uint8_t shifted_lastKit2Bytes(KitData *kd, uint8_t **data);
};

#endif /* DISPLAY_H_ */
