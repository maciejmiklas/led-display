#include "AnimatedTextArea.h"

AnimatedTextArea::AnimatedTextArea(Display *display, pixel_t boxWidth, uint16_t animationDelayMs, uint8_t tid) :
		TextArea(display, boxWidth), tid(tid), driver(NULL), animationDelayMs(animationDelayMs) {
	this->lastFrameTimeMs = 0;
}

void AnimatedTextArea::init() {
	driver = createDriver();
}
void AnimatedTextArea::cycle() {
#if FRAME_DELAY
	uint32_t time = ms();
	if (animationDelayMs > time - lastFrameTimeMs) {
		return;
	}
	lastFrameTimeMs = time;
#endif

#if LOG_TA
	log(F("Next frame(%d)"), tid);
#endif

	driver->execute();
}

AnimatedTextArea::~AnimatedTextArea() {
}

boolean AnimatedTextArea::isRunning() {
	return driver->isRunning();
}

void AnimatedTextArea::stop() {
	driver->changeState(StateMashine::STATE_NOOP);
}

void AnimatedTextArea::resume() {
	driver->changeState(0);
}

void AnimatedTextArea::resetState() {
	driver->reset();
}
