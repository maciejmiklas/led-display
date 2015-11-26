#include "TextArea.h"

TextArea::TextArea(Display *display, pixel_t boxWidth) :
		boxWidth(boxWidth), xDataSize(boxWidth / KIT_DIM + 2), yDataSize(KIT_DIM) {
	this->display = display;
	this->data = alloc2DArray8(FONT8_HEIGHT, xDataSize);

#if DEBUG_TA
	debug(F("Created %dx%d text area"), FONT8_HEIGHT, boxWidth);
#endif

}

TextArea::~TextArea() {
	delete2DArray(data, FONT8_HEIGHT, xDataSize);
}
