#include "TextArea8x8.h"

TextArea8x8::TextArea8x8(Display *display, pixel boxWidth, uint16_t animationDelayMs) :
		animationDelayMs(animationDelayMs), boxWidth(boxWidth), xDataSize(boxWidth / KIT_DIM + 2), yDataSize(KIT_DIM) {
	this->display = display;
	this->data = alloc2DArray8(FONT8_HEIGHT, xDataSize);

#if DEBUG_TA
	debug(F("Created 8x8 area with data: %dx%d, width: %d"), FONT8_HEIGHT, xDataSize, boxWidth);
#endif

	debug(F("ANIM DELAY: %d"), this->animationDelayMs);
}

TextArea8x8::~TextArea8x8() {
	delete2DArray(data, FONT8_HEIGHT, xDataSize);
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

void TextArea8x8::scroll(pixel x, pixel y, uint8_t chars, ...) {
#if DEBUG_TA
	debug(F("Scroll text on (%d,%d) with %d chars"), x, y, chars);
#endif

	va_list va;
	va_start(va, chars);
	const uint8_t xDataBufIdx = xDataSize - 1;
	for (uint8_t charIdx = 0; charIdx < chars; charIdx++) {
		uint8_t fontIdx = va_arg(va, int);

#if DEBUG_TA
		debug(F("Font %d"), fontIdx);
#endif

		// copy next font into off screen bye on the left
		font8x8_copy(data, xDataBufIdx, fontIdx);

		// scroll 8 bits from left to right
		for (uint8_t wIdx = 0; wIdx < FONT8_WIDTH; wIdx++) {
			for (uint8_t hIdx = 0; hIdx < FONT8_WIDTH; hIdx++) {
				shiftL(data[hIdx], xDataSize);
			}
#if DEBUG_TA
			debug(F("Paint font line %d"), wIdx);
#endif
			display->paint(x, y, boxWidth, 8, data);
			delay(animationDelayMs);
		}
	}

	// scrolling of all fonts is done, now scroll currently displayed fonts to the left until display is empty
	for (uint8_t wIdx = 0; wIdx < boxWidth; wIdx++) {
		for (uint8_t hIdx = 0; hIdx < FONT8_WIDTH; hIdx++) {
			shiftL(data[hIdx], xDataSize);
		}
#if DEBUG_TA
		debug(F("Paint font line %d"), wIdx);
#endif
		display->paint(x, y, boxWidth, 8, data);
		delay(animationDelayMs);
	}

#if DEBUG_TA
	debug(F("Final animation"));
#endif

	va_end(va);
}

