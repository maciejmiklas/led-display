#include "Scrolling8x8TextArea.h"

Scrolling8x8TextArea::Scrolling8x8TextArea(Display *display, pixel_t boxWidth, uint16_t animationDelayMs, uint8_t id) :
		AnimatedTextArea(display, boxWidth, animationDelayMs, id), mainState(this), charState(this), endState(this), __machineDriver(
				3, &mainState, &charState, &endState) {

	this->x = 0;
	this->y = 0;
	this->text = NULL;
	this->loop = false;
}

Scrolling8x8TextArea::~Scrolling8x8TextArea() {

}

MachineDriver* Scrolling8x8TextArea::createDriver() {
	return &__machineDriver;
}

void Scrolling8x8TextArea::paint() {
	display->paint(x, y, boxWidth, 8, data);
}

void Scrolling8x8TextArea::scroll(pixel_t x, pixel_t y, boolean loop, char *text) {
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
Scrolling8x8TextArea::MainState::MainState(Scrolling8x8TextArea* sta) :
		sta(sta) {
	this->charsIdx = 0;
}

Scrolling8x8TextArea::MainState::~MainState() {
}

void Scrolling8x8TextArea::MainState::reset() {
	charsIdx = 0;
}

boolean Scrolling8x8TextArea::MainState::isIntermediate() {
	return true;
}

void Scrolling8x8TextArea::MainState::init() {
}

uint8_t Scrolling8x8TextArea::MainState::execute() {
	char nextChar = sta->text[charsIdx++];
#if LOG_TA
	log(F("Next char: %d on %d"), nextChar, charsIdx);
#endif

	if (nextChar == '\0') {
		return Scrolling8x8TextArea::STATE_END;
	}
	// copy next font into first off screen bye on the right
	font8x8_copy(sta->data, sta->xDataOffScrIdx, nextChar);
	return STATE_CHAR;
}

// ################ CharState ################
Scrolling8x8TextArea::CharState::CharState(Scrolling8x8TextArea* sta) :
		sta(sta) {
	this->wIdx = 0;
}

Scrolling8x8TextArea::CharState::~CharState() {
}

boolean Scrolling8x8TextArea::CharState::isIntermediate() {
	return false;
}

void Scrolling8x8TextArea::CharState::reset() {
	init();
}

void Scrolling8x8TextArea::CharState::init() {
	wIdx = 0;
}

uint8_t Scrolling8x8TextArea::CharState::execute() {
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
		return Scrolling8x8TextArea::STATE_MAIN;
	}

	return StateMashine::STATE_NOCHANGE;
}

// ################ EndState ################
Scrolling8x8TextArea::EndState::EndState(Scrolling8x8TextArea* sta) :
		sta(sta) {
	this->charsIdx = 0;
}

Scrolling8x8TextArea::EndState::~EndState() {
}

boolean Scrolling8x8TextArea::EndState::isIntermediate() {
	return false;
}

void Scrolling8x8TextArea::EndState::reset() {
	init();
}

void Scrolling8x8TextArea::EndState::init() {
	charsIdx = 0;
}

uint8_t Scrolling8x8TextArea::EndState::execute() {
	if (charsIdx == sta->boxWidth + FONT8_WIDTH) {
		uint8_t state;
		if (sta->loop) {
			state = StateMashine::STATE_RESET;
		} else {
			state = StateMashine::STATE_NOOP;
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

	return StateMashine::STATE_NOCHANGE;
}

