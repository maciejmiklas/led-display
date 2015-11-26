#ifndef SCROLLINGTEXTAREA_H_
#define SCROLLINGTEXTAREA_H_

#include "AnimatedTextArea.h"
#include "StateMashine.h"

class ScrollingTextArea: public AnimatedTextArea {

public:
	ScrollingTextArea(Display *display, pixel_t boxWidth, uint16_t animationDelayMs, uint8_t id);
	void scroll(pixel_t x, pixel_t y, boolean loop, uint8_t chars, ...);

protected:
	virtual void nextFrame();
	virtual void stop();

private:
	enum state_t {
		STATE_NOCHANGE = 0, STATE_MAIN = 1, STATE_CHAR = 2, STATE_END = 3, STATE_NOOP = 4
	};

	pixel_t x;
	pixel_t y;
	uint8_t charsSize;
	uint8_t *chars;
	boolean loop;
	StateMashine *stateMashine;
	void freeScChars();
	void changeState(state_t state);

	/**
	 * Scrolls main part of the text, meaning the whole text starting with empty display and
	 * ends when last letter appears on the left part of the display.
	 */
	class MainState: public StateMashine {
	public:
		MainState(ScrollingTextArea& sta);
		virtual uint8_t execute();
		virtual void init();
	private:
		uint8_t charsIdx;
		ScrollingTextArea& sta;
	};
	MainState *mainState;

	/**
	 * Sub-state of Main State - it's responsible for scrolling of a single character.
	 */
	class CharState: public StateMashine {
	public:
		CharState(ScrollingTextArea& sta);
		virtual uint8_t execute();
		virtual void init();
	private:
		ScrollingTextArea& sta;
		uint8_t wIdx;
	};
	CharState *charState;

	/**
	 * Scrolls characters remaining in screen buffer to the end of the display. This is the ending animation, once it's
	 * finished display is clear, because all characters has scrolled out of it.
	 * Remember also that we have one char in off screen buffer
	 */
	class EndState: public StateMashine {
	public:
		EndState(ScrollingTextArea& sta);
		virtual uint8_t execute();
		virtual void init();
	private:
		uint8_t charsIdx;
		ScrollingTextArea& sta;
	};
	EndState *endState;

	/**
	 * Does nothing ;)
	 */
	class NoopState: public StateMashine {
	public:
		NoopState();
		virtual uint8_t execute();
		virtual void init();
	};
	NoopState *noopState;
};

#endif /* SCROLLINGTEXTAREA_H_ */
