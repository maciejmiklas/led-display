#include <StaticTextArea.h>

StaticTextArea::StaticTextArea(Display *display, pixel_t boxWidth) :
		TextArea(display, boxWidth) {
}

void StaticTextArea::box(pixel_t x, pixel_t y, uint8_t text[][8], uint8_t textChars) {
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

void StaticTextArea::box(pixel_t x, pixel_t y, uint8_t chars, ...) {
#if DEBUG_TA
	debug(F("Display text box on (%d,%d) with %d chars"), x, y, chars);
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
