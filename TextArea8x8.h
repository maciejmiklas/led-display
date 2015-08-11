#ifndef LD_TextArea8x8_h
#define LD_TextArea8x8_h

#include "Arduino.h"
#include "Display.h"
#include "Util.h"
#include "Font8x8.h"

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
	void box(pixel x, pixel y, uint8_t text[][8], uint8_t textChars);

	/**
	 * Displays text in a box placed on display on (x,y) and limited by #boxWidth, height is set to 8.
	 *
	 * The var-arg parameter is expected to be array of uint8_t where each one is an offset to a character stored
	 * in #FONT8_ASCI. In order to print text: "Hi :)" on (10,12) you would call box(10, 12, 72, 105, 58, 41).
	 */
	void box(pixel x, pixel y, uint8_t vaLength, ...);

private:
	Display *display;

	/** Width in pixels of a box. */
	pixel boxWidth;

	/** Screen data. First position in #data array indicates row, second represents pixels within this row. */
	uint8_t **data;

	/** Amount of columns (width) in 2D data table. */
	uint8_t dataColumns;

	void copyChar(uint8_t dataIdx, uint8_t fontIdx);

};

#endif /* LD_TextArea8x8_h */
