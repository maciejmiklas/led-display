#ifndef TEXTSPRITE_H_
#define TEXTSPRITE_H_

#include "Arduino.h"
#include "Display.h"

/**
 * This class provides various possibilities to display and animate 8x8 font.
 * Font has to be stored in array in a special format - it's description can be found in: Font8x8.h
 */
class Area {
public:

	/**
	 * Displays text in box placed on display on (x,y) and limited by #width, height is set to 8.
	 *
	 * #text is a two dimensional array containing characters to be displayed. The amount of characters is given by
	 * #textLenght. First dimension of #text contains pointers to fonts, each font is stored in array containing 8
	 * bytes
	 */
	void box(uint8_t x, uint8_t y, uint8_t width, uint8_t** text, uint8_t textLenght);

private:

};

#endif /* TEXTSPRITE_H_ */
