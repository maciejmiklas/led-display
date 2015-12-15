#ifndef SCROLLINGTEXTAREA_H_
#define SCROLLINGTEXTAREA_H_

#include "AnimatedTextArea.h"
#include "StateMashine.h"
#include "MachineDriver.h"

class ScrollingTextArea: public AnimatedTextArea {

public:
	ScrollingTextArea(Display* display, pixel_t boxWidth, uint16_t animationDelayMs, uint8_t id);
	void scroll(pixel_t x, pixel_t y, boolean loop, char* text);
	virtual ~ScrollingTextArea();
protected:
	virtual void onStop();
	virtual MachineDriver* createDriver();

private:
	enum state_t {
		STATE_MAIN = 0, STATE_CHAR = 1, STATE_END = 2
	};
	pixel_t x;
	pixel_t y;
	char* text;

	/** plays text animation in a loop. In this case #isRunning() always returns true. */
	boolean loop;
	void freeScChars();
	void paint();

	/**
	 * Scrolls main part of the text, meaning the whole text starting with empty display and
	 * ends when last letter appears on the left part of the display.
	 */
	class MainState: public StateMashine {
	public:
		MainState(ScrollingTextArea* sta);
		virtual ~MainState();
		virtual uint8_t execute();
		virtual void init();
	private:
		uint8_t charsIdx;
		ScrollingTextArea* sta;
	};

	/**
	 * Sub-state of Main State - it's responsible for scrolling of a single character.
	 */
	class CharState: public StateMashine {
	public:
		CharState(ScrollingTextArea* sta);
		virtual ~CharState();
		virtual uint8_t execute();
		virtual void init();
	private:
		ScrollingTextArea* sta;
		uint8_t wIdx;
	};

	/**
	 * Scrolls characters remaining in screen buffer to the end of the display. This is the ending animation, once it's
	 * finished display is clear, because all characters has scrolled out of it.
	 * Remember also that we have one char in off screen buffer
	 */
	class EndState: public StateMashine {
	public:
		virtual ~EndState();
		EndState(ScrollingTextArea* sta);
		virtual uint8_t execute();
		virtual void init();
	private:
		uint8_t charsIdx;
		ScrollingTextArea* sta;
	};

	MainState mainState;
	CharState charState;
	EndState endState;
	MachineDriver machineDriver;
};

#endif /* SCROLLINGTEXTAREA_H_ */
