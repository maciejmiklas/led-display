#include "StaticText8x8.h"

StaticText8x8::StaticText8x8(Canvas *canvas, pixel_t boxWidth) :
		canvas(canvas), boxWidth(boxWidth), xDataSize(boxWidth / KIT_DIM + 2), data(
				alloc2DArray8(FONT8_HEIGHT, xDataSize)) {
}

void StaticText8x8::box(pixel_t x, pixel_t y, char const *text) {
#if LOG
	log(F("Display text box on (%d,%d)"), x, y);
#endif

	clean2DArray8(data, FONT8_HEIGHT, xDataSize);

	uint8_t charsIdx = 0;
	uint8_t dataIdx = 0;
	char nextChar = text[charsIdx++];
	while (nextChar != '\0') {
		font8x8_copy(data, dataIdx, nextChar);
		dataIdx++;
		nextChar = text[charsIdx++];
		if (dataIdx == xDataSize) {
			break;
		}
	}
	canvas->paint(x, y, boxWidth, 8, data);
}

void StaticText8x8::clearDisplay() {
	clean2DArray8(data, FONT8_HEIGHT, xDataSize);
}

StaticText8x8::~StaticText8x8() {
	delete2DArray8(data);
}
