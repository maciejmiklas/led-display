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
 #ifndef CANVAS_H_
#define CANVAS_H_

#include "Arduino.h"

/** One pixel - single LED. */
typedef uint8_t pixel_t;

/**
 * Dimension in pixels (LEDs) of LED-Matrix-Kit.
 * Currently #KIT_DIM is fixed to 8, because pixel data is stored in byte array and we are assuming,
 * that one byte fully covers one row on single LED-Kit.
 */
const static uint8_t KIT_DIM = 8;

/**
 * Represents a rectangular area of the screen onto which the application can pain.
 */
class Canvas {

public:
	/**
	 * Paints given matrix on the display starting at pixel on (x,y) position - this position is relative to top left
	 * corner of whole display.
	 * Matrix dimension in pixels is given by #width and #height. This gives us a following rectangle:
	 * (x,y)-(#width-1,#height-1).
	 *
	 * Painted data can exceed display size, in this case it will be trimmed.
	 *
	 * Matrix data is stored in two dimensional array of bytes (#data), where single bit represents one pixel.
	 * First position in #data array indicates row, second represents pixels within this row: #data[y][x]. Each byte
	 * contains 8 pixels. For example: structure consisting of 5 rows and 14 pixels per row has dimension: data[5][2] -
	 * we need two bytes per row to express 14 pixels. Also second byte of each row uses only first 6 bits -> 8 + 6 = 14
	 *
	 * Size of #data: #data[0][0] - #data[#height-1][#width/8]
	 *
	 * For example, matrix consisting of 3x2 bytes has maximal dimension 16(2*8) x 3 pixels,
	 * which gives us a 48 pixels in total. We have 3 rows, each one has two bytes.
	 */
	virtual void paint(pixel_t x, pixel_t y, pixel_t width, pixel_t height, uint8_t **data) = 0;

protected:
	Canvas();
	virtual ~Canvas();
};

#endif /* CANVAS_H_ */
