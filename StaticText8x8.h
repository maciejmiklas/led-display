#ifndef STATIC8x8TEXTAREA_H_
#define STATIC8x8TEXTAREA_H_

#include "Display.h"
#include "Util.h"
#include "Font8x8.h"

/**
 * Contains methods showing static text within given box. Each method displays text once and that's it.
 *
 * This class can only display 8x8 font. There are two reasons for it: the size was fine and the implementation easy.
 * Display consist of vertical lines (rows) where each one is represented by byte array, and each byte in this array
 * controls 8 pixels. Font size is also 8 - meaning that we can directly copy fonts into display without byte shifting.
 */
class StaticText8x8 {
public:

	/**
	 * #boxWidth is the width in pixels. Height is always set to 8 pixels - it's determined by character size.
	 */
	StaticText8x8(Display *display, pixel_t boxWidth);
	~StaticText8x8();

	/**
	 * Displays text in a box placed on display on (x,y) and limited by #boxWidth, height is set to 8.
	 *
	 * The vararg parameter is expected to be array of uint8_t where each one is an offset to a character stored
	 * in #FONT8_ASCI. In order to print text: "Hi :)" on (10,12) you would call
	 * #box(10, 12, 6, 72, 105, 32, 58, 41). "72, 105, 32, 58, 41" ==  "Hi :)" and 6 is the amount of vararg params.
	 */
	void box(pixel_t x, pixel_t y, uint8_t chars, ...);
	void clearDisplay();

private:
	Display * const display;

	/** Width in pixels of a box. */
	const pixel_t boxWidth;

	/**
	 * Amount of columns (width) in #data (2D table). This is an amount of bytes in horizontal position.
	 * For example: each font is build of 8x8 pixels, so it occupies 8 rows and one column in our #data table.
	 */
	const uint8_t xDataSize;

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
	uint8_t ** const data;
};

#endif /* STATICTEXTAREA_H_ */
