#include "AnimatedText8x8.h"

AnimatedText8x8::AnimatedText8x8(Canvas *canvas, pixel_t boxWidth, uint16_t animationDelayMs, uint8_t tid) :
		canvas(canvas), tid(tid), boxWidth(boxWidth), data(alloc2DArray8(FONT8_HEIGHT, xDataSize)), xDataSize(
				boxWidth / KIT_DIM + 2), yDataSize(KIT_DIM), xDataOffScrIdx(xDataSize - 1), driver(NULL), lastFrameTimeMs(
				0), animationDelayMs(animationDelayMs) {
}

void AnimatedText8x8::init() {
	driver = createDriver();
}
void AnimatedText8x8::cycle() {
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

void AnimatedText8x8::clearDisplay() {
	clean2DArray8(data, FONT8_HEIGHT, xDataSize);
}

AnimatedText8x8::~AnimatedText8x8() {
	delete2DArray8(data);
}

boolean AnimatedText8x8::isRunning() {
	return driver->isRunning();
}

void AnimatedText8x8::stop() {
	driver->changeState(StateMachine::STATE_NOOP);
}

void AnimatedText8x8::resume() {
	driver->changeState(0);
}

void AnimatedText8x8::resetState() {
	driver->reset();
}
