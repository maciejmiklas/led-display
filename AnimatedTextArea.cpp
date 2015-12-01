#include "AnimatedTextArea.h"

AnimatedTextArea::AnimatedTextArea(Display *display, pixel_t boxWidth, uint16_t animationDelayMs, uint8_t tid) :
		TextArea(display, boxWidth), tid(tid), driver(NULL), animationDelayMs(animationDelayMs) {
	this->lastFrameTimeMs = 0;
}

void AnimatedTextArea::cycle() {
	if (driver == NULL) {
		driver = createDriver();
	}
	uint32_t time = ms();
	if (animationDelayMs > time - lastFrameTimeMs) {
		return;
	}
#if DEBUG_TA
	debug(F("Next frame(%d)"), tid);
#endif
	driver->execute();
	lastFrameTimeMs = time;
}

AnimatedTextArea::~AnimatedTextArea() {
	delete (driver);
}

boolean AnimatedTextArea::isRunning() {
	return driver->isRunning();
}

void AnimatedTextArea::stop() {
	driver->changeState(StateMashine::STATE_NOOP);
	onStop();
}

void AnimatedTextArea::resetState() {
	if (driver == NULL) {
		driver = createDriver();
	}
	driver->changeState(0);
}
