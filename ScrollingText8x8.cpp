/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "ScrollingText8x8.h"

ScrollingText8x8::ScrollingText8x8(Canvas *canvas, pixel_t boxWidth, uint16_t animationDelayMs, uint8_t id) :
		AnimatedText8x8(canvas, boxWidth, animationDelayMs, id), x(0), y(0), text(NULL), mode(SINGLE_PASS), mainState(
				this), charState(this), endState(this), listener(NULL), __machineDriver(3, &mainState, &charState,
				&endState) {
}

ScrollingText8x8::~ScrollingText8x8() {

}

MachineDriver* ScrollingText8x8::createDriver() {
	return &__machineDriver;
}

void ScrollingText8x8::setListener(Listener *listener) {
	this->listener = listener;
}

void ScrollingText8x8::paint() {
	canvas->paint(x, y, boxWidth, 8, data);
}

void ScrollingText8x8::scroll(pixel_t x, pixel_t y, mode_t mode, char const *text) {
#if LOG_D
	log(F("STX  on (%d,%d)"), x, y);
#endif

	this->x = x;
	this->y = y;
	this->mode = mode;
	this->text = text;
	resetState();
	clearDisplay();
}

// ################ MainState ################
ScrollingText8x8::MainState::MainState(ScrollingText8x8* sta) :
		sta(sta) {
	this->charsIdx = 0;
}

ScrollingText8x8::MainState::~MainState() {
}

void ScrollingText8x8::MainState::reset() {
	charsIdx = 0;
}

boolean ScrollingText8x8::MainState::isIntermediate() {
	return true;
}

void ScrollingText8x8::MainState::init() {
}

uint8_t ScrollingText8x8::MainState::execute() {
	char nextChar = sta->text[charsIdx++];
#if LOG_D
	log(F("STX NC %d on %d"), nextChar, charsIdx);
#endif

	if (nextChar == '\0') {
		if (sta->mode == CONTINOUS_LOOP) {
			if (sta->listener != NULL) {
				sta->listener->onScrollEnd();
			}
			return StateMachine::STATE_RESET;
		}
		return ScrollingText8x8::STATE_END;
	}
	// copy next font into first off screen bye on the right
	font8x8_copy(sta->data, sta->xDataOffScrIdx, nextChar);
	return STATE_CHAR;
}

// ################ CharState ################
ScrollingText8x8::CharState::CharState(ScrollingText8x8* sta) :
		sta(sta) {
	this->wIdx = 0;
}

ScrollingText8x8::CharState::~CharState() {
}

boolean ScrollingText8x8::CharState::isIntermediate() {
	return false;
}

void ScrollingText8x8::CharState::reset() {
	init();
}

void ScrollingText8x8::CharState::init() {
	wIdx = 0;
}

uint8_t ScrollingText8x8::CharState::execute() {
	// scroll 8 bits from left to right
	for (uint8_t hIdx = 0; hIdx < FONT8_HEIGHT; hIdx++) {
		shiftL(sta->data[hIdx], sta->xDataSize);
	}
#if LOG_D
	log(F("STX shift: %d"), wIdx);
#endif
	sta->paint();

	wIdx++;
	if (wIdx == FONT8_WIDTH) {
		return ScrollingText8x8::STATE_MAIN;
	}

	return StateMachine::STATE_NOCHANGE;
}

// ################ EndState ################
ScrollingText8x8::EndState::EndState(ScrollingText8x8* sta) :
		sta(sta) {
	this->charsIdx = 0;
}

ScrollingText8x8::EndState::~EndState() {
}

boolean ScrollingText8x8::EndState::isIntermediate() {
	return false;
}

void ScrollingText8x8::EndState::reset() {
	init();
}

void ScrollingText8x8::EndState::init() {
	charsIdx = 0;
}

uint8_t ScrollingText8x8::EndState::execute() {
	if (charsIdx == sta->boxWidth + FONT8_WIDTH) {
		uint8_t state;
		if (sta->mode == LOOP) {
			if (sta->listener != NULL) {
				sta->listener->onScrollEnd();
			}
			state = StateMachine::STATE_RESET;
		} else {
			state = StateMachine::STATE_NOOP;
		}
		return state;
	}
	charsIdx++;
	for (uint8_t hIdx = 0; hIdx < FONT8_WIDTH; hIdx++) {
		shiftL(sta->data[hIdx], sta->xDataSize);
	}
#if LOG_D
	log(F("STX ES %d"), charsIdx);
#endif
	sta->paint();

	return StateMachine::STATE_NOCHANGE;
}

// ################ ScrollingTextListener ################
ScrollingText8x8::Listener::Listener() {

}

ScrollingText8x8::Listener::~Listener() {

}
