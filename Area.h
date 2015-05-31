#ifndef AREA_H_
#define AREA_H_

#include "Arduino.h"

class Display {
public:

	/**
	 * Continuously scrolls text from right to left within a box. Its width is given by #width and height is set
	 * to 8 pixels.
	 * The box's top left corner is placed on display on (x,y) coordinates.
	 *
	 * The variable #fonts points to character array containing codes of fonts from Font.h. Each font has
	 * variable width (up to 8 pixels) and height set to 8 pixels.
	 */
	void animateText(uint8_t x, uint8_t y, uint8_t width, uint8_t length, uint8_t* fonts);

private:

};

#endif /* AREA_H_ */
