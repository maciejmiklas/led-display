#ifndef STATIC8x8TEXTAREA_H_
#define STATIC8x8TEXTAREA_H_

#include <TextArea.h>

/**
 * Contains methods showing static text within given box. Each method displays text once and that's it.
 *
 * This class can only display 8x8 font. There are two reasons for it: the size was fine and the implementation easy.
 * Display consist of vertical lines (rows) where each one is represented by byte array, and each byte in this array
 * controls 8 pixels. Font size is also 8 - meaning that we can directly copy fonts into display without byte shifting.
 */
class Static8x8TextArea: public TextArea {
public:

	/**
	 * #boxWidth is the width in pixels. Height is always set to 8 pixels - it's determined by character size.
	 */
	Static8x8TextArea(Display *display, pixel_t boxWidth);

	/**
	 * Displays text in a box placed on display on (x,y) and limited by #boxWidth, height is set to 8.
	 *
	 * The vararg parameter is expected to be array of uint8_t where each one is an offset to a character stored
	 * in #FONT8_ASCI. In order to print text: "Hi :)" on (10,12) you would call
	 * #box(10, 12, 6, 72, 105, 32, 58, 41). "72, 105, 32, 58, 41" ==  "Hi :)" and 6 is the amount of vararg params.
	 */
	void box(pixel_t x, pixel_t y, uint8_t chars, ...);
};

#endif /* STATICTEXTAREA_H_ */
