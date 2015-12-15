#include "ScrollingTextArea.h"

ScrollingTextArea::ScrollingTextArea(Display *display, pixel_t boxWidth, uint16_t animationDelayMs, uint8_t id) :
		AnimatedTextArea(display, boxWidth, animationDelayMs, id), mainState(this), charState(this), endState(this),
		machineDriver(3, &mainState, &charState, &endState){

	this->x = 0;
	this->y = 0;
	this->text = NULL;
	this->loop = false;
}

ScrollingTextArea::~ScrollingTextArea() {

}

MachineDriver* ScrollingTextArea::createDriver() {
	return &machineDriver;
}

void ScrollingTextArea::onStop() {
	freeScChars();
}

void ScrollingTextArea::paint() {
	display->paint(x, y, boxWidth, 8, data);
}

void ScrollingTextArea::scroll(pixel_t x, pixel_t y, boolean loop, char *text) {
#if DEBUG_TA
	debug(F("Scroll text on (%d,%d) with %d chars"), x, y, text);
#endif

	freeScChars();

	this->x = x;
	this->y = y;
	this->loop = loop;
	this->text = text;

	resetState();
}

void ScrollingTextArea::freeScChars() {
	if (text == NULL) {
		return;
	}
	text = NULL;
}

// ################ MainState ################
ScrollingTextArea::MainState::MainState(ScrollingTextArea* sta) :
		sta(sta) {
	this->charsIdx = 0;
}

ScrollingTextArea::MainState::~MainState() {
}

void ScrollingTextArea::MainState::init() {
	charsIdx = 0;
}

uint8_t ScrollingTextArea::MainState::execute() {
	char nextChar = sta->text[charsIdx++];
#if DEBUG_TA
	debug(F("Next char: %d"), nextChar);
#endif

	if (nextChar == '\0') {
		return ScrollingTextArea::STATE_END;
	}
	// copy next font into first off screen bye on the right
	font8x8_copy(sta->data, sta->xDataOffScrIdx, nextChar);
	return STATE_CHAR;
}

// ################ CharState ################
ScrollingTextArea::CharState::CharState(ScrollingTextArea* sta) :
		sta(sta) {
	this->wIdx = 0;
}

ScrollingTextArea::CharState::~CharState() {
}

void ScrollingTextArea::CharState::init() {
	wIdx = 0;
}

uint8_t ScrollingTextArea::CharState::execute() {
	// scroll 8 bits from left to right
	for (uint8_t hIdx = 0; hIdx < FONT8_HEIGHT; hIdx++) {
		shiftL(sta->data[hIdx], sta->xDataSize);
	}
#if DEBUG_TA
	debug(F("Display shift: %d"), wIdx);
#endif
	sta->paint();

	wIdx++;
	if (wIdx == FONT8_WIDTH) {
		return ScrollingTextArea::STATE_MAIN;
	}

	return StateMashine::STATE_NOCHANGE;
}

// ################ EndState ################
ScrollingTextArea::EndState::EndState(ScrollingTextArea* sta) :
		sta(sta) {
	this->charsIdx = 0;
}

ScrollingTextArea::EndState::~EndState() {
}

void ScrollingTextArea::EndState::init() {
	charsIdx = 0;
}

uint8_t ScrollingTextArea::EndState::execute() {
	if (charsIdx == sta->boxWidth + FONT8_WIDTH) {
		uint8_t state;
		if (sta->loop) {
			state = ScrollingTextArea::STATE_MAIN;
		} else {
			state = StateMashine::STATE_NOOP;
		}
		return state;
	}
	charsIdx++;
	for (uint8_t hIdx = 0; hIdx < FONT8_WIDTH; hIdx++) {
		shiftL(sta->data[hIdx], sta->xDataSize);
	}
#if DEBUG_TA
	debug(F("Display end shift: %d"), charsIdx);
#endif
	sta->paint();

	return StateMashine::STATE_NOCHANGE;
}

