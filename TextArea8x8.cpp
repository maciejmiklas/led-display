#include "TextArea8x8.h"

TextArea8x8::TextArea8x8(Display *display, pixel boxWidth) {
	this->display = display;
	this->boxWidth = boxWidth;
	this->dataColumns = boxWidth / KIT_DIM + 1;
	this->data = alloc2DArray8(FONT8_HEIGHT, dataColumns);

#if DEBUG_TA
	debug(F("Created 8x8 area with data: %dx%d, width: %d"), FONT8_HEIGHT, dataColumns, boxWidth);
#endif
}

TextArea8x8::~TextArea8x8() {
	delete2DArray(data, FONT8_HEIGHT, dataColumns);
}

void TextArea8x8::box(pixel x, pixel y, uint8_t text[][8], uint8_t textChars) {
	for (uint8_t charIdx = 0; charIdx < textChars; charIdx++) {
		for (uint8_t row = 0; row < 8; row++) {
			data[row][charIdx] = text[charIdx][row];
#if DEBUG_TA
			debug(F("Font data[%d][%d] = 0x%02x"), row, charIdx, data[row][charIdx]);
#endif
		}
	}
	display->paint(x, y, boxWidth, 8, data);
}

void TextArea8x8::box(pixel x, pixel y, uint8_t chars, ...) {
#if DEBUG_TA
	debug(F("Paint box on (%d,%d) with %d chars"), x, y, chars);
#endif

	// TODO clean only data elements that are not used to paint - each font takes 8x8, so it fully wipes out data block
	clean2DArray8(data, FONT8_HEIGHT, dataColumns);

	va_list va;
	va_start(va, chars);

	for (uint8_t charIdx = 0; charIdx < chars; charIdx++) {
		uint8_t fontIdx = va_arg(va, int);
		font8x8_copy(data, charIdx, fontIdx);
	}
	display->paint(x, y, boxWidth, 8, data);
	va_end(va);
}
