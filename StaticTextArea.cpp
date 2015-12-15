#include "StaticTextArea.h"

StaticTextArea::StaticTextArea(Display *display, pixel_t boxWidth) :
		TextArea(display, boxWidth) {
}

void StaticTextArea::box(pixel_t x, pixel_t y, uint8_t chars, ...) {
#if DEBUG_TA
	log(F("Display text box on (%d,%d) with %d chars"), x, y, chars);
#endif

	clean2DArray8(data, 0, chars - 1, FONT8_HEIGHT, xDataSize);

	va_list va;
	va_start(va, chars);

	for (uint8_t charIdx = 0; charIdx < chars; charIdx++) {
		uint8_t fontIdx = va_arg(va, int);
		font8x8_copy(data, charIdx, fontIdx);
	}
	display->paint(x, y, boxWidth, 8, data);
	va_end(va);
}
