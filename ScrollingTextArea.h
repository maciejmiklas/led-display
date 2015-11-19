#ifndef SCROLLINGTEXTAREA_H_
#define SCROLLINGTEXTAREA_H_

#include <AnimatedTextArea.h>

class ScrollingTextArea: public AnimatedTextArea {

public:
	ScrollingTextArea(Display *display, pixel boxWidth, uint16_t animationDelayMs, uint8_t id);

	void scroll(pixel x, pixel y, boolean loop, uint8_t chars, ...);

protected:
	virtual void nextFrame();
	virtual void stop();

private:
	pixel x;
	pixel y;
	uint8_t charsSize;
	uint8_t *chars;
	uint8_t charsIdx;
	boolean loop;
	uint8_t wIdx;

	void freeScChars();

	typedef void (ScrollingTextArea::*STATE)(void);
	STATE state;

	/**
	 * Scrolls main part of the text, meaning the whole text starting with empty display and
	 * ends when last letter appears on the left part of the display.
	 */
	void state_scrollMain_init();
	void state_scrollMain();

	void state_scrollChar();

	/**
	 * Scrolls characters remaining in screen buffer to the end of the display. This is the ending animation.
	 * Remember also that we have one char in off screen buffer
	 */
	void state_scrollEnd_init();
	void state_scrollEnd();

	void noop();
};

#endif /* SCROLLINGTEXTAREA_H_ */
