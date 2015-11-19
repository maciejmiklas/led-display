#include <ScrollingTextArea.h>

ScrollingTextArea::ScrollingTextArea(Display *display, pixel boxWidth, uint16_t animationDelayMs, uint8_t id) :
		AnimatedTextArea(display, boxWidth, animationDelayMs, id) {

	this->charsSize = 0;
	this->state = &ScrollingTextArea::noop;
	this->x = 0;
	this->y = 0;
	this->chars = NULL;
	this->charsIdx = 0;
	this->loop = false;
	this->wIdx = 0;
}

void ScrollingTextArea::freeScChars() {
	if (chars == NULL) {
		return;
	}
	delete (chars);
	chars = NULL;
	charsSize = 0;
}

void ScrollingTextArea::stop() {
	this->state = &ScrollingTextArea::noop;
}

void ScrollingTextArea::noop() {
}

void ScrollingTextArea::state_scrollMain_init() {
#if DEBUG_TA
	debug(F("Execute: state_scrollMain_init"));
#endif

	charsIdx = 0;
	state = &ScrollingTextArea::state_scrollMain;
}

void ScrollingTextArea::state_scrollMain() {
#if DEBUG_TA
	debug(F("Execute: state_scrollMain"));
#endif
	if (charsIdx == charsSize) {
		state = &ScrollingTextArea::state_scrollEnd_init;
		return;
	}
	const uint8_t xDataBufIdx = xDataSize - 1;
	uint8_t nextChar = chars[charsIdx++];
	wIdx = 0;
#if DEBUG_TA
	debug(F("Next char %d"), nextChar);
#endif

	state = &ScrollingTextArea::state_scrollChar;

	// copy next font into first off screen bye on the right
	font8x8_copy(data, xDataBufIdx, nextChar);
}

void ScrollingTextArea::state_scrollChar() {
#if DEBUG_TA
	debug(F("Execute: state_scrollChar"));
#endif

	// scroll 8 bits from left to right
	for (uint8_t hIdx = 0; hIdx < FONT8_HEIGHT; hIdx++) {
		shiftL(data[hIdx], xDataSize);
	}
#if DEBUG_TA
	debug(F("Paint font line %d"), wIdx);
#endif
	display->paint(x, y, boxWidth, 8, data);

	wIdx++;
	if (wIdx == FONT8_WIDTH) {
		state = &ScrollingTextArea::state_scrollMain;
	}
}

void ScrollingTextArea::state_scrollEnd() {
#if DEBUG_TA
	debug(F("Execute: state_scrollEnd"));
#endif
	if (charsIdx == boxWidth + FONT8_WIDTH) {
		if (loop) {
			state = &ScrollingTextArea::state_scrollMain_init;
		} else {
			state = &ScrollingTextArea::noop;
		}
		return;
	}
	charsIdx++;
	for (uint8_t hIdx = 0; hIdx < FONT8_WIDTH; hIdx++) {
		shiftL(data[hIdx], xDataSize);
	}
#if DEBUG_TA
	debug(F("Paint font line %d"), charsIdx);
#endif
	display->paint(x, y, boxWidth, 8, data);
	//delay(animationDelayMs);
}

void ScrollingTextArea::state_scrollEnd_init() {
#if DEBUG_TA
	debug(F("Execute: state_scrollEnd_init"));
#endif

	charsIdx = 0;
	state = &ScrollingTextArea::state_scrollEnd;
}

void ScrollingTextArea::scroll(pixel x, pixel y, boolean loop, uint8_t charsSize, ...) {
#if DEBUG_TA
	debug(F("Scroll text on (%d,%d) with %d chars"), x, y, chars);
#endif

	freeScChars();

	this->x = x;
	this->y = y;
	this->loop = loop;
	this->charsSize = charsSize;
	this->chars = new uint8_t[charsSize];

	// copy fonts from varargs into #scChars
	va_list va;
	va_start(va, charsSize);
	for (uint8_t charIdx = 0; charIdx < charsSize; charIdx++) {
		this->chars[charIdx] = va_arg(va, int);
	}
	va_end(va);

	state = &ScrollingTextArea::state_scrollMain_init;
}

void ScrollingTextArea::nextFrame() {
#if DEBUG_TA
	debug(F("Next frame in Text Area"));
#endif
	(this->*state)();
}
