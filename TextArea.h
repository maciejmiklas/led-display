#ifndef LD_TextArea_h
#define LD_TextArea_h

#include "Arduino.h"
#include "Display.h"
#include "Util.h"
#include "Font8x8.h"

#define LOG_TA false

/**
 * This class provides various possibilities to display and animate 8x8 font.
 * Font has to be stored in array in a special format - it's description can be found in: Font8x8.h
 */
class TextArea {
public:
	virtual ~TextArea();

protected:
	TextArea(Display *display, pixel_t boxWidth);
	Display *display;
	void clearDisplay();

	/** Width in pixels of a box. */
	const pixel_t boxWidth;

	/**
	 * Screen data representing content of this Text Area. First position in #data array indicates row (y axis),
	 * second represents pixels within this row.
	 *
	 * #data has 8 rows and #xDataSize columns. Each column is a byte - so it automatically holds 8 pixels.
	 *
	 * Traversing #data array by first dimension we are moving on the display from top to the bottom pixel by pixel.
	 * Traversing #data by its second dimension we are moving on the display from left to the right by 8 pixels with
	 * each byte.
	 *
	 * #data heiht is set t 8, because we are using such font. #data width is calculated based on width of
	 * scrolling area - this is a multiplication of 8 plus extra byte for offscreen buffer. This gives us following
	 * bounds: #data[0][0] - #data[8][#boxWidth / 8 + 2].
	 */
	uint8_t **data;

	/**
	 * Amount of columns (width) in #data (2D table). This is an amount of bytes in horizontal position.
	 * For example: each font is build of 8x8 pixels, so it occupies 8 rows and one column in our #data table.
	 */
	const uint8_t xDataSize;

	/** Amount of rows (height) in 2D data table. */
	const uint8_t yDataSize;

	/** Position to offscreen buffer byte of #data. */
	const uint8_t xDataOffScrIdx;

	void copyChar(uint8_t dataIdx, uint8_t fontIdx);
};

#endif /* LD_TextArea_h */
