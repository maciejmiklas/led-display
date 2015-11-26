#ifndef LD_TextArea_h
#define LD_TextArea_h

#include "Arduino.h"
#include "Display.h"
#include "Util.h"
#include "Font8x8.h"

#define DEBUG_TA false

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

	/** Width in pixels of a box. */
	const pixel_t boxWidth;

	/**
	 * Screen data. First position in #data array indicates row (y axis), second represents pixels within this row.
	 * #data has 8 rows and #xDataSize columns.
	 */
	uint8_t **data;

	/**
	 * Amount of columns (width) in 2D data table. This is an amount of bytes in horizontal position. Each font has
	 * 8x8 pixels, so it occupies one byte horizontal and 8 rows.
	 * Horizontal size of the data is big enough to cover display box given by #boxWidth plus one byte for off screen
	 * buffer.
	 * This byte is used as buffer for vertical scrolling animation.
	 */
	const uint8_t xDataSize;

	/** Amount of rows (height) in 2D data table. */
	const uint8_t yDataSize;

	void copyChar(uint8_t dataIdx, uint8_t fontIdx);
};

#endif /* LD_TextArea_h */
