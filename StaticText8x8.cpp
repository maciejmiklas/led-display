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
 #include "StaticText8x8.h"

StaticText8x8::StaticText8x8(Canvas *canvas, pixel_t boxWidth) :
		canvas(canvas), boxWidth(boxWidth), xDataSize(boxWidth / KIT_DIM + 2), data(
				alloc2DArray8(FONT8_HEIGHT, xDataSize)) {
}

void StaticText8x8::box(pixel_t x, pixel_t y, char const *text) {
#if LOG_D
	log(F("STT n (%d,%d)"), x, y);
#endif

	clean2DArray8(data, FONT8_HEIGHT, xDataSize);

	uint8_t charsIdx = 0;
	uint8_t dataIdx = 0;
	char nextChar = text[charsIdx++];
	while (nextChar != '\0') {
		font8x8_copy(data, dataIdx, nextChar);
		dataIdx++;
		nextChar = text[charsIdx++];
		if (dataIdx == xDataSize) {
			break;
		}
	}
	canvas->paint(x, y, boxWidth, 8, data);
}

void StaticText8x8::clearDisplay() {
	clean2DArray8(data, FONT8_HEIGHT, xDataSize);
}

StaticText8x8::~StaticText8x8() {
	delete2DArray8(data);
}
