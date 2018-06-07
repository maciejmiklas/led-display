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
 #ifndef STATIC8x8TEXTAREA_H_
#define STATIC8x8TEXTAREA_H_

#include "DispUtil.h"
#include "Canvas.h"
#include "Font8x8.h"

/**
 * Contains methods showing static text within given box. Each method displays text once and that's it.
 *
 * This class can only display 8x8 font. There are two reasons for it: the size was fine and the implementation easy.
 * Display consist of vertical lines (rows) where each one is represented by byte array, and each byte in this array
 * controls 8 pixels. Font size is also 8 - meaning that we can directly copy fonts into display without byte shifting.
 */
class StaticText8x8 {
public:

	/**
	 * #boxWidth is the width in pixels. Height is always set to 8 pixels - it's determined by character size.
	 */
	StaticText8x8(Canvas *canvas, pixel_t boxWidth);
	~StaticText8x8();

	/**
	 * Displays text in a box placed on display on (x,y) and limited by #boxWidth, height is set to 8.
	 */
	void box(pixel_t x, pixel_t y, char const *text);
	void clearDisplay();

private:
	Canvas * const canvas;

	/** Width in pixels of a box. */
	const pixel_t boxWidth;

	/**
	 * Amount of columns (width) in #data (2D table). This is an amount of bytes in horizontal position.
	 * For example: each font is build of 8x8 pixels, so it occupies 8 rows and one column in our #data table.
	 */
	const uint8_t xDataSize;

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
};

#endif /* STATICTEXTAREA_H_ */
