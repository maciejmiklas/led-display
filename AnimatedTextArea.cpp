#include <AnimatedTextArea.h>

AnimatedTextArea::AnimatedTextArea(Display *display, pixel boxWidth, uint16_t animationDelayMs, uint8_t id) :
		TextArea(display, boxWidth), animationDelayMs(animationDelayMs), id(id) {
	this->lastFrameTimeMs = 0;
}

void AnimatedTextArea::cycle() {
	uint32_t time = ms();
	if (animationDelayMs > time - lastFrameTimeMs) {
		return;
	}
#if DEBUG_TA
	debug(F("Next frame(%d)"), id);
#endif
	nextFrame();
	lastFrameTimeMs = time;
}
