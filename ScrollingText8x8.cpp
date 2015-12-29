#include "ScrollingText8x8.h"

ScrollingText8x8::ScrollingText8x8(Canvas *canvas, pixel_t boxWidth, uint16_t animationDelayMs, uint8_t id) :
		AnimatedText8x8(canvas, boxWidth, animationDelayMs, id), x(0), y(0), text(NULL), loop(false), mainState(this), charState(
				this), endState(this), __machineDriver(3, &mainState, &charState, &endState) {
}

ScrollingText8x8::~ScrollingText8x8() {

}

MachineDriver* ScrollingText8x8::createDriver() {
	return &__machineDriver;
}

void ScrollingText8x8::paint() {
	canvas->paint(x, y, boxWidth, 8, data);
}

void ScrollingText8x8::scroll(pixel_t x, pixel_t y, boolean loop, char *text) {
#if LOG_TA
	log(F("Scroll text on (%d,%d)"), x, y);
#endif

	this->x = x;
	this->y = y;
	this->loop = loop;
	this->text = text;
	resetState();
	clearDisplay();
}

// ################ MainState ################
ScrollingText8x8::MainState::MainState(ScrollingText8x8* sta) :
		sta(sta) {
	this->charsIdx = 0;
}

ScrollingText8x8::MainState::~MainState() {
}

void ScrollingText8x8::MainState::reset() {
	charsIdx = 0;
}

boolean ScrollingText8x8::MainState::isIntermediate() {
	return true;
}

void ScrollingText8x8::MainState::init() {
}

uint8_t ScrollingText8x8::MainState::execute() {
	char nextChar = sta->text[charsIdx++];
#if LOG_TA
	log(F("Next char: %d on %d"), nextChar, charsIdx);
#endif

	if (nextChar == '\0') {
		return ScrollingText8x8::STATE_END;
	}
	// copy next font into first off screen bye on the right
	font8x8_copy(sta->data, sta->xDataOffScrIdx, nextChar);
	return STATE_CHAR;
}

// ################ CharState ################
ScrollingText8x8::CharState::CharState(ScrollingText8x8* sta) :
		sta(sta) {
	this->wIdx = 0;
}

ScrollingText8x8::CharState::~CharState() {
}

boolean ScrollingText8x8::CharState::isIntermediate() {
	return false;
}

void ScrollingText8x8::CharState::reset() {
	init();
}

void ScrollingText8x8::CharState::init() {
	wIdx = 0;
}

uint8_t ScrollingText8x8::CharState::execute() {
	// scroll 8 bits from left to right
	for (uint8_t hIdx = 0; hIdx < FONT8_HEIGHT; hIdx++) {
		shiftL(sta->data[hIdx], sta->xDataSize);
	}
#if LOG_TA
	log(F("Display shift: %d"), wIdx);
#endif
	sta->paint();

	wIdx++;
	if (wIdx == FONT8_WIDTH) {
		return ScrollingText8x8::STATE_MAIN;
	}

	return StateMachine::STATE_NOCHANGE;
}

// ################ EndState ################
ScrollingText8x8::EndState::EndState(ScrollingText8x8* sta) :
		sta(sta) {
	this->charsIdx = 0;
}

ScrollingText8x8::EndState::~EndState() {
}

boolean ScrollingText8x8::EndState::isIntermediate() {
	return false;
}

void ScrollingText8x8::EndState::reset() {
	init();
}

void ScrollingText8x8::EndState::init() {
	charsIdx = 0;
}

uint8_t ScrollingText8x8::EndState::execute() {
	if (charsIdx == sta->boxWidth + FONT8_WIDTH) {
		uint8_t state;
		if (sta->loop) {
			state = StateMachine::STATE_RESET;
		} else {
			state = StateMachine::STATE_NOOP;
		}
		return state;
	}
	charsIdx++;
	for (uint8_t hIdx = 0; hIdx < FONT8_WIDTH; hIdx++) {
		shiftL(sta->data[hIdx], sta->xDataSize);
	}
#if LOG_TA
	log(F("EndState charsIdx: %d"), charsIdx);
#endif
	sta->paint();

	return StateMachine::STATE_NOCHANGE;
}

