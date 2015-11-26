/*
 * StaticTextArea.h
 *
 *  Created on: Sep 28, 2015
 *      Author: mmiklas
 */

#ifndef STATICTEXTAREA_H_
#define STATICTEXTAREA_H_

#include <TextArea.h>

/**
 * Contains methods showing static text within given box. Each method displays text once and that's it.
 */
class StaticTextArea: public TextArea {
public:

	/**
	 * #boxWidth is the width in pixels. Height is always set to 8 pixels - it's determined by character size.
	 */
	StaticTextArea(Display *display, pixel_t boxWidth);

	/**
	 * Displays text in a box placed on display on (x,y) and limited by #boxWidth, height is set to 8.
	 *
	 * #text is a two dimensional array containing characters to be displayed. The amount of characters is given by
	 * #textChars. First dimension of #text contains characters, each one is stored in array containing 8
	 * bytes (one font). For example: #text[0][8] points to first character, #text[1][8] to second,
	 * #text[5][8] to fifth.
	 */
	void box(pixel_t x, pixel_t y, uint8_t text[][8], uint8_t textChars);

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
