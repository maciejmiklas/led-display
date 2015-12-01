#include "ScrollingTextArea.h"

ScrollingTextArea::ScrollingTextArea(Display* display, pixel_t boxWidth, uint16_t animationDelayMs, uint8_t id) :
		AnimatedTextArea(display, boxWidth, animationDelayMs, id) {

	this->charsSize = 0;
	this->x = 0;
	this->y = 0;
	this->chars = NULL;
	this->loop = false;
}

MachineDriver* ScrollingTextArea::createDriver() {
	ScrollingTextArea::MainState *mainState = new ScrollingTextArea::MainState(*this);
	ScrollingTextArea::CharState *charState = new ScrollingTextArea::CharState(*this);
	ScrollingTextArea::EndState *endState = new ScrollingTextArea::EndState(*this);
	MachineDriver* machineDriver = new MachineDriver(3, mainState, charState, endState);
	return machineDriver;
}

void ScrollingTextArea::onStop() {
	freeScChars();
}

void ScrollingTextArea::paintBuffer() {
	display->paint(x, y, boxWidth, 8, data);
}

void ScrollingTextArea::scroll(pixel_t x, pixel_t y, boolean loop, uint8_t charsSize, ...) {
#if DEBUG_TA
	debug(F("Scroll text on (%d,%d) with %d chars"), x, y, chars);
#endif

	freeScChars();

	this->x = x;
	this->y = y;
	this->loop = loop;
	this->charsSize = charsSize;
	this->chars = new uint8_t[charsSize];

	// copy fonts from var-args into #scChars
	va_list va;
	va_start(va, charsSize);
	for (uint8_t charIdx = 0; charIdx < charsSize; charIdx++) {
		this->chars[charIdx] = va_arg(va, int);
	}
	va_end(va);

	resetState();
}

// ################ MainState ################
ScrollingTextArea::MainState::MainState(ScrollingTextArea& sta) :
		sta(sta) {
	this->charsIdx = 0;
}

void ScrollingTextArea::MainState::init() {
	charsIdx = 0;
}

uint8_t ScrollingTextArea::MainState::execute() {
	if (charsIdx == sta.charsSize) {
		return ScrollingTextArea::STATE_END;
	}
	const uint8_t xDataBufIdx = sta.xDataSize - 1;
	uint8_t nextChar = sta.chars[charsIdx++];
#if DEBUG_TA
	debug(F("Next char: %d"), nextChar);
#endif

	// copy next font into first off screen bye on the right
	font8x8_copy(sta.data, xDataBufIdx, nextChar);
	return STATE_CHAR;
}

void ScrollingTextArea::freeScChars() {
	if (chars == NULL) {
		return;
	}
	delete (chars);
	chars = NULL;
	charsSize = 0;
}

// ################ CharState ################
ScrollingTextArea::CharState::CharState(ScrollingTextArea& sta) :
		sta(sta) {
	this->wIdx = 0;
}
void ScrollingTextArea::CharState::init() {
	wIdx = 0;
}

uint8_t ScrollingTextArea::CharState::execute() {
	// scroll 8 bits from left to right
	for (uint8_t hIdx = 0; hIdx < FONT8_HEIGHT; hIdx++) {
		shiftL(sta.data[hIdx], sta.xDataSize);
	}
#if DEBUG_TA
	debug(F("Display shift: %d"), wIdx);
#endif
	sta.paintBuffer();

	wIdx++;
	if (wIdx == FONT8_WIDTH) {
		return ScrollingTextArea::STATE_MAIN;
	}

	return StateMashine::STATE_NOCHANGE;
}

// ################ EndState ################
ScrollingTextArea::EndState::EndState(ScrollingTextArea& sta) :
		sta(sta) {
	this->charsIdx = 0;
}

void ScrollingTextArea::EndState::init() {
	charsIdx = 0;
}

uint8_t ScrollingTextArea::EndState::execute() {
	if (charsIdx == sta.boxWidth + FONT8_WIDTH) {
		uint8_t state;
		if (sta.loop) {
			state = ScrollingTextArea::STATE_MAIN;
		} else {
			state = StateMashine::STATE_NOOP;
		}
		return state;
	}
	charsIdx++;
	for (uint8_t hIdx = 0; hIdx < FONT8_WIDTH; hIdx++) {
		shiftL(sta.data[hIdx], sta.xDataSize);
	}
#if DEBUG_TA
	debug(F("Display end shift: %d"), wIdx);
#endif
	sta.paintBuffer();

	return StateMashine::STATE_NOCHANGE;
}

