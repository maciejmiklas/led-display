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
#ifndef ANIMATEDTEXTAREA_H_
#define ANIMATEDTEXTAREA_H_

#include "DispUtil.h"
#include "MachineDriver.h"
#include "Font8x8.h"
#include "Canvas.h"

#define FRAME_DELAY true

/**
 * Subclasses of this class are displaying animated text within a box.
 *
 */
class AnimatedText8x8 {

public:
	/*
	 * Animation consists of multiple frames where each one has to be displayed with right timing. The methods playing
	 * particular animation are non blocking -> by calling method #animateXyz(....) we are initializing animation, in
	 * order to play it you have to call method #cycle() - this method will ensure that next frame is being displayed
	 * with right timing.
	 */
	void cycle();

	/** returns true if animation is currently being played. */
	boolean isRunning();

	void stop();
	void resume();
	void init();
	void clearDisplay();

protected:
	AnimatedText8x8(Canvas *canvas, pixel_t boxWidth, uint16_t animationDelayMs, uint8_t tid);
	virtual ~AnimatedText8x8();

	Canvas * const canvas;

	/** Unique id used for logging */
	const uint8_t tid;

	/** Created driver will be deleted in ~AnimatedText8x8() */
	virtual MachineDriver* createDriver() = 0;
	void resetState();

	/** Width in pixels of a box. */
	const pixel_t boxWidth;

	/**
	 * Amount of columns (width) in #data (2D table). This is an amount of bytes in horizontal position.
	 * For example: each font is build of 8x8 pixels, so it occupies 8 rows and one column in our #data table.
	 */
	const uint8_t xDataSize;

	/** Amount of rows (height) in 2D data table. */
	const uint8_t yDataSize;

	/**
	 * Screen data representing content of this Text Area. First position in #data array indicates row (y axis),
	 * second represents pixels within this row.
	 *
	 * #data has 8 rows and #xDataSize columns. Each column is a byte - so it automatically holds 8 pixels.
	 *
	 * Traversing #data array by first dimension we are moving on the display from top to the bottom pixel by pixel.
	 * Traversing #data by its second dimension we are moving on the display from left to the right by 8 pixels with
	 * each byte.
	 *
	 * #data heiht is set t 8, because we are using such font. #data width is calculated based on width of
	 * scrolling area - this is a multiplication of 8 plus extra byte for offscreen buffer. This gives us following
	 * bounds: #data[0][0] - #data[8][#boxWidth / 8 + 2].
	 */
	uint8_t ** const data;

	/** Position to offscreen buffer byte of #data. */
	const uint8_t xDataOffScrIdx;

private:
	MachineDriver *driver;

	/** Time when the last frame has been animated. */
	uint32_t lastFrameTimeMs;

	const uint32_t animationDelayMs;

	void copyChar(uint8_t dataIdx, uint8_t fontIdx);
};

#endif /* ANIMATEDTEXTAREA_H_ */
