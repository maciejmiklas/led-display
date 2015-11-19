#ifndef ANIMATEDTEXTAREA_H_
#define ANIMATEDTEXTAREA_H_

#include "TextArea.h"
#include "Util.h"

/**
 * Subclasses of this class are displaying animated text within a box.
 *
 */
class AnimatedTextArea: public TextArea {

public:
	/*
	 * Animation consists of multiple frames where each one has to be displayed with right timing. The methods playing
	 * particular animation are non blocking -> by calling method #animateXyz(....) we are initializing animation, in
	 * order to play it you have to call method #cycle() - this method will ensure that next frame is being displayed
	 * with right timing.
	 */
	void cycle();
	virtual void stop() const = 0;

protected:
	AnimatedTextArea(Display *display, pixel boxWidth, uint16_t animationDelayMs, uint8_t id);
	virtual void nextFrame() const = 0;

	/** Unique id used for logging */
	const uint8_t id;

private:
	/** Time when the last frame has been animated. */
	uint16_t lastFrameTimeMs;
	const uint16_t animationDelayMs;
};

#endif /* ANIMATEDTEXTAREA_H_ */
