#ifndef TEXT_AREA8x8_H
#define TEXT_AREA8x8_H

#include "Arduino.h"
#include "Display.h"
#include "Util.h"

#define DEBUG_TA true

/**
 * This class provides various possibilities to display and animate 8x8 font.
 * Font has to be stored in array in a special format - it's description can be found in: Font8x8.h
 */
class TextArea8x8 {
public:
	TextArea8x8(Display *display, pixel boxWidth);

	~TextArea8x8();
	/**
	 * Displays text in a box placed on display on (x,y) and limited by #boxWidth, height is set to 8.
	 *
	 * #text is a two dimensional array containing characters to be displayed. The amount of characters is given by
	 * #textChars. First dimension of #text contains characters, each one is stored in array containing 8
	 * bytes (one font). For example: #text[0][8] points to first character, #text[1][8] to second,
	 * #text[5][8] to fifth.
	 */
	void box(pixel x, pixel y, uint8_t** text, uint8_t textChars);

private:
	Display *display;

	/** Width in pixels of a box. */
	pixel boxWidth;

	/** Screen data. */
	uint8_t **data;

	/** Amount of columns (width) in 2D data table. */
	uint8_t dataColumns;

};

#endif /* TEXT_AREA8x8_H */
