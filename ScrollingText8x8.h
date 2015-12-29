#ifndef SCROLLING8x8TEXTAREA_H_
#define SCROLLING8x8TEXTAREA_H_

#include "AnimatedText8x8.h"
#include "StateMachine.h"
#include "MachineDriver.h"

/**
 * Displays text in a box and scrolls it from right to left.
 *
 * This class can only display 8x8 font. There are two reasons for it: the size was fine and the implementation easy.
 * Display consist of vertical lines (rows) where each one is represented by byte array, and each byte in this array
 * controls 8 pixels. Font size is also 8 - meaning that we can directly copy fonts into display without byte shifting.
 */
class ScrollingText8x8: public AnimatedText8x8 {

public:
	ScrollingText8x8(Canvas* canvas, pixel_t boxWidth, uint16_t animationDelayMs, uint8_t id);
	void scroll(pixel_t x, pixel_t y, boolean loop, char* text);
	virtual ~ScrollingText8x8();
protected:
	MachineDriver* createDriver();

private:
	enum state_t {
		STATE_MAIN = 0, STATE_CHAR = 1, STATE_END = 2
	};
	pixel_t x;
	pixel_t y;
	char* text;

	/** plays text animation in a loop. In this case #isRunning() always returns true. */
	boolean loop;
	void paint();

	/**
	 * Scrolls main part of the text, meaning the whole text starting with empty display and
	 * ends when last letter appears on the left part of the display.
	 */
	class MainState: public StateMachine {
	public:
		MainState(ScrollingText8x8* sta);
		virtual ~MainState();
		virtual uint8_t execute();
		virtual void init();
		virtual void reset();
		virtual boolean isIntermediate();
	private:
		uint8_t charsIdx;
		ScrollingText8x8* sta;
	};

	/**
	 * Sub-state of Main State - it's responsible for scrolling of a single character.
	 */
	class CharState: public StateMachine {
	public:
		CharState(ScrollingText8x8* sta);
		virtual ~CharState();
		virtual uint8_t execute();
		virtual void init();
		virtual void reset();
		virtual boolean isIntermediate();
	private:
		ScrollingText8x8* sta;
		uint8_t wIdx;
	};

	/**
	 * Scrolls characters remaining in screen buffer to the end of the display. This is the ending animation, once it's
	 * finished display is clear, because all characters has scrolled out of it.
	 * Remember also that we have one char in off screen buffer
	 */
	class EndState: public StateMachine {
	public:
		virtual ~EndState();
		EndState(ScrollingText8x8* sta);
		virtual uint8_t execute();
		virtual void init();
		virtual void reset();
		virtual boolean isIntermediate();
	private:
		uint8_t charsIdx;
		ScrollingText8x8* sta;
	};

	MainState mainState;
	CharState charState;
	EndState endState;

	/** Variable should be never used due to OO design. We just have to keep references to driver to avoid GC. */
	MachineDriver __machineDriver;
};

#endif /* SCROLLING8x8TEXTAREA_H_ */
