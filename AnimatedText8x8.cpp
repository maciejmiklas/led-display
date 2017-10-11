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
#include "AnimatedText8x8.h"

AnimatedText8x8::AnimatedText8x8(Canvas *canvas, pixel_t boxWidth, uint16_t animationDelayMs, uint8_t tid) :
		canvas(canvas), tid(tid), boxWidth(boxWidth), xDataSize(boxWidth / KIT_DIM + 2), yDataSize(KIT_DIM), data(
				alloc2DArray8(FONT8_HEIGHT, xDataSize)), xDataOffScrIdx(xDataSize - 1), driver(NULL), lastFrameTimeMs(
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

#if LOG_D
	log(F("AT FR %d"), tid);
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
