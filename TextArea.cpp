#include "TextArea.h"

TextArea::TextArea(Display *display, pixel_t boxWidth) :
		boxWidth(boxWidth), xDataSize(boxWidth / KIT_DIM + 2), yDataSize(KIT_DIM), xDataOffScrIdx(xDataSize - 1) {
	this->display = display;
	this->data = alloc2DArray8(FONT8_HEIGHT, xDataSize);
#if DEBUG_TA
	log(F("Created %dx%d text area"), boxWidth, FONT8_HEIGHT);
#endif

}

TextArea::~TextArea() {
	delete2DArray(data, FONT8_HEIGHT);
}
