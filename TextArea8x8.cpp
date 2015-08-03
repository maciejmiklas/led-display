#include "TextArea8x8.h"

TextArea8x8::TextArea8x8(Display *display, pixel boxWidth) {
	this->display = display;
	this->boxWidth = boxWidth;
	this->dataColumns = boxWidth / KIT_DIM + 1;
	this->data = alloc2DArray8(8, dataColumns);

#if DEBUG_TA
	debug(F("Created 8x8 area with dataColumns: %d and boxWidth: %d"), dataColumns, boxWidth);
#endif
}

TextArea8x8::~TextArea8x8() {
	delete2DArray(data, 8, dataColumns);
}

void TextArea8x8::box(pixel x, pixel y, uint8_t** text, uint8_t textChars) {
	for (uint8_t charIdx = 0; charIdx < textChars; charIdx++) {
		for (uint8_t row = 0; row < 8; row++) {
			data[row][charIdx] = text[charIdx][row];
		}
	}
	display->paint(x, y, boxWidth, 8, data);
}
