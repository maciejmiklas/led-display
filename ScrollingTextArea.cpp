#include <ScrollingTextArea.h>

ScrollingTextArea::ScrollingTextArea(Display *display, pixel_t boxWidth, uint16_t animationDelayMs, uint8_t id) :
		AnimatedTextArea(display, boxWidth, animationDelayMs, id) {

	this->charsSize = 0;
	this->x = 0;
	this->y = 0;
	this->chars = NULL;
	this->loop = false;
	this->mainState = new ScrollingTextArea::MainState(*this);
	this->endState = new ScrollingTextArea::EndState(*this);
	this->charState = new ScrollingTextArea::CharState(*this);
	this->noopState = new ScrollingTextArea::NoopState();
	this->stateMashine = noopState;
}

void ScrollingTextArea::stop() {
	changeState(STATE_NOOP);
	freeScChars();
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

	changeState(STATE_MAIN);
}

void ScrollingTextArea::changeState(state_t state) {
	switch (state) {
	case STATE_NOCHANGE:
		return;

	case STATE_MAIN:
#if DEBUG_TA
		debug(F("Next state: STATE_MAIN"));
#endif
		stateMashine = mainState;
		break;

	case STATE_CHAR:
#if DEBUG_TA
		debug(F("Next state: STATE_CHAR"));
#endif
		stateMashine = charState;
		break;

	case STATE_END:
#if DEBUG_TA
		debug(F("Next state: STATE_END"));
#endif
		stateMashine = endState;
		break;

	default:
	case STATE_NOOP:
#if DEBUG_TA
		debug(F("Next state: STATE_NOOP"));
#endif
		stateMashine = noopState;
		break;
	}

	stateMashine->init();
}

void ScrollingTextArea::nextFrame() {
#if DEBUG_TA
	debug(F("Next frame in Text Area"));
#endif

	uint8_t stateInt = stateMashine->execute();
	changeState(static_cast<state_t>(stateInt));
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
	debug(F("Next char %d"), nextChar);
#endif

	// copy next font into first off screen bye on the right
	font8x8_copy(sta.data, xDataBufIdx, nextChar);
	return ScrollingTextArea::STATE_NOCHANGE;
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
	debug(F("Paint font line %d"), wIdx);
#endif
	sta.display->paint(sta.x, sta.y, sta.boxWidth, 8, sta.data);

	wIdx++;
	if (wIdx == FONT8_WIDTH) {
		return ScrollingTextArea::STATE_MAIN;
	}

	return ScrollingTextArea::STATE_NOCHANGE;
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
			state = ScrollingTextArea::STATE_NOOP;
		}
		return state;
	}
	charsIdx++;
	for (uint8_t hIdx = 0; hIdx < FONT8_WIDTH; hIdx++) {
		shiftL(sta.data[hIdx], sta.xDataSize);
	}
#if DEBUG_TA
	debug(F("Paint font line %d"), charsIdx);
#endif
	sta.display->paint(sta.x, sta.y, sta.boxWidth, 8, sta.data);

	return ScrollingTextArea::STATE_NOCHANGE;
}

// ################ NoopState ################
ScrollingTextArea::NoopState::NoopState() {
}

void ScrollingTextArea::NoopState::init() {
}

uint8_t ScrollingTextArea::NoopState::execute() {
	return ScrollingTextArea::STATE_NOCHANGE;
}
