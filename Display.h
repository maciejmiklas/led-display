#ifndef LD_Display_h
#define LD_Display_h

#include "Arduino.h"
#include "SPI.h"
#include "Log.h"
#include "Canvas.h"

#define LOG_DI false
#define SIMULATE_DI true

/**
 * Double buffer ensures that only changed bytes are send to Max. This improves performance and increases memory
 * footprint. It needs 8 bytes for each Max chip plus space for array holding pointers to each row on the display -
 * basically we have keep copy on #data in RAM.
 */
#define DEOUBLE_BUFFER true

/** One LED-Kit - 8x8 LED Matrix. */
typedef uint8_t kit_t;

/** Slave Select line for max7219. */
typedef uint8_t ss_t;

// max7219 registers
const static uint8_t REG_NOOP = 0x0;
const static uint8_t REG_DIGIT0 = 0x1;
const static uint8_t REG_DIGIT1 = 0x2;
const static uint8_t REG_DIGIT2 = 0x3;
const static uint8_t REG_DIGIT3 = 0x4;
const static uint8_t REG_DIGIT4 = 0x5;
const static uint8_t REG_DIGIT5 = 0x6;
const static uint8_t REG_DIGIT6 = 0x7;
const static uint8_t REG_DIGIT7 = 0x8;
const static uint8_t REG_DECODEMODE = 0x9;
const static uint8_t REG_INTENSITY = 0xA;
const static uint8_t REG_SCANLIMIT = 0xB;
const static uint8_t REG_SHUTDOWN = 0xC;
const static uint8_t REG_DISPLAYTEST = 0xF;

/**
 * Display consist of a matrix of 8x8-LED-Kits. For example putting 8x5 kits together will build
 * a display with (8*8)x(8*5) pixels -> (64x40).
 *
 * Paint operations on display do not send immediately data to MAX. First flush call does it. This gives us possibility
 * for multiple modifications of display content and one hardware repaint operation.
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
 *
 */
class Display: public Canvas {
public:

	Display(kit_t xKits, kit_t yKits, ss_t **ss);
	~Display();

	void paint(pixel_t x, pixel_t y, pixel_t width, pixel_t height, uint8_t **data);

	/** Initialized SPI and 8x8-Matrix elements. */
	void setup();

	/** Clears whole display. */
	void clear();

	void clear(pixel_t x, pixel_t y, pixel_t width, pixel_t height);

	/** Sends the content of screen buffer to MAX chips and repaints whole display. */
	void flush();
private:

	/** Horizontal amount of 8x8-Matrices, known as Kits */
	const kit_t xKits;

	/** Vertical amount of 8x8-Matrices */
	const kit_t yKits;

	const uint8_t rows;

	/**
	 * Slave Select lines reflecting positions of 8x8-Matrices on the Displays. It has the same orientation as
	 * the #screen.
	 * The first coordinate in table indicates vertical position, second horizontal: #ss[vertical][horizontal],
	 * this gives us dimensions: #ss[0][0] -> ss[yKits-1][xKits-1]
	 */
	ss_t ** const ss;

	/**
	 * It's a two dimensional array containing pixels for LED-Kits. Each LED-Kit has embedded memory, but we need to
	 * keep pixel state in RAM in order to calculate row value for case when more then one sprite has to be displayed
	 * on single LED-Kit.
	 * Print method displays single sprite that eventually crosses few LED-Kits, we have only to set pixels for this
	 * sprite without affecting it direct neighbors. We have to calculate byte (8 pixels) for single row on
	 * particular kit that contains pixels for already painted sprite and for new one.
	 *
	 * First dimension in #screen indicates row, second kit (#screen[row][kit]). For example #screen[2][3] indicates
	 * 3rd row on 4th kit (starting form 0).
	 * Going over rows (first dimension) we are traversing the display from top to bottom, pixel by pixel.
	 * Traversing over second dimension of #screen array moves us by 8 pixels with each byte.
	 */
	uint8_t ** const screen;

	/**
	 * We are using double screen buffer in order to send to MAX only bytes that has changed.
	 */
	uint8_t **screenCmp;

	/**
	 * Contains data for each LED-Kit. Painting procedure goes over all kits and KitData will get recalculated
	 * for each Kit.
	 */
	typedef struct {
		// all (x,y) coordinates are starting from 0 and are inclusive.

		/** starting pixels on first LED-Kit */
		pixel_t xOnFirstKit;
		pixel_t yOnFirstKit;

		/** (x,y) position starting from first Kit that we are panting on. */
		kit_t xRelKit;
		kit_t yRelKit;

		/** Amount of LED-Kits used for painting */
		kit_t xRelKitSize;
		kit_t yRelKitSize;

		/**
		 * Amount of bytes from #data containing pixels for x-axis (vertical) - second argument of #data[y][x].
		 * Amount is already limited so that it only covers Kits that will be used for painting.
		 */
		uint8_t xDataBytes;

		/** (x,y) Kit position starting from first Kit in LED-Kit-Matrix. */
		kit_t xKit;
		kit_t yKit;

		/** (x,y) position on kit that we are painting on. */
		pixel_t xOnKit;
		pixel_t yOnKit;

		/** dimensions on kit that we are painting on. */
		pixel_t xOnKitSize;
		pixel_t yOnKitSize;

		/** (x,y) for #screen[y,x]  */
		uint8_t xOnScreenIdx;
		uint8_t yOnScreenIdx;

		// y position on #data
		uint8_t yDataIdx;
		int aaa;
	} KitData;

	KitData _kd;
	KitData _kdCopy;

	void setupMax();
	void setupMax(ss_t ss);
	void setupSpi();
	void clearKit(ss_t ss);

	inline KitData* createKitData();
	inline KitData* copyKitData();

	inline void send(ss_t ss, uint8_t address, uint8_t value);

	/** reduces width/height so it fits on the screen */
	inline pixel_t limitSize(pixel_t xy, pixel_t wh, kit_t startKitXY, kit_t endKitXY);

	/** finds ending 8x8-Matrix - inclusive */
	inline kit_t calcEndKit(pixel_t xy, pixel_t wh, kit_t yxKits);

	/** calculates width/height within current kit */
	inline pixel_t calcSizeOnKit(pixel_t xy, pixel_t wh, kit_t xyKit, kit_t xyOnKit, kit_t startKitXY, kit_t endKitXY);

	/** #kd will get modified inside function ! */
	inline void paintOnKitVal(KitData kd, uint8_t **data);
	inline void paintOnKitRef(KitData *kd, uint8_t **data);

	// overlapped_xxx -> vertical position of data is not shifted relatively to first kit. Data consists of 8 bit
	// values and those align perfectly with 8 LED rows
	inline uint8_t overlapped_firstAndMidleKit(KitData &kd, uint8_t **data);
	inline uint8_t overlapped_lastKit(KitData &kd, uint8_t **data);

	// shifted_xxx -> vertical position of data IS shifted relatively to first kit
	inline uint8_t shifted_firstKit(KitData &kd, uint8_t **data);

	/**
	 *  Vertical position on first kit is shifted, so we need two bytes of data to cover single row on one LED-Kit.
	 *  This method will be used to paint row on middle kit.
	 */
	inline uint8_t shifted_middleKit(KitData &kd, uint8_t **data);

	/**
	 *  This is exception to method using 2 bytes. On the last kit it might be sufficient to use only one byte to
	 *  print single row, because #width limits amount of pixel within row.
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
	inline uint8_t shifted_lastKit1Byte(KitData &kd, uint8_t **data);

	inline uint8_t shifted_lastKit2Bytes(KitData &kd, uint8_t **data);
};

#endif /* LD_Display_h */
