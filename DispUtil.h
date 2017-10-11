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
#ifndef ARD_DISP_UTIL_H
#define ARD_DISP_UTIL_H

#include "Arduino.h"
#include "ArdLog.h"
void dutil_setup();
void dutil_cycle();

uint32_t ms();

inline void clean2DArray8(uint8_t **array, uint8_t rows, uint8_t cols);

inline void sort_8t(uint8_t arr[], uint8_t size) {
	uint8_t i, temp, j;
	for (i = 1; i < size; i++) {
		temp = arr[i];
		j = i - 1;
		while (temp < arr[j] && j != 0) {
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = temp;
	}
}

inline uint16_t getFreeRam() {
	extern int __heap_start, *__brkval;
	int v;
	return (uint16_t) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

inline uint16_t abs_t(int16_t val) {
	return val > 0 ? val : val * -1;
}

inline uint8_t abs_t(int8_t val) {
	return val > 0 ? val : val * -1;
}

inline int16_t map16(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

inline uint8_t maskL(uint8_t bits) {
	uint8_t bv;

	switch (bits) {
	default:
	case 0:
		bv = B00000000;
		break;

	case 1:
		bv = B10000000;
		break;

	case 2:
		bv = B11000000;
		break;

	case 3:
		bv = B11100000;
		break;

	case 4:
		bv = B11110000;
		break;

	case 5:
		bv = B11111000;
		break;

	case 6:
		bv = B11111100;
		break;

	case 7:
		bv = B11111110;
		break;

	case 8:
		bv = B11111111;
		break;
	}
	return bv;
}

inline uint8_t maskR(uint8_t bits) {
	uint8_t bv;

	switch (bits) {
	default:
	case 0:
		bv = B00000000;
		break;

	case 1:
		bv = B00000001;
		break;

	case 2:
		bv = B00000011;
		break;

	case 3:
		bv = B00000111;
		break;

	case 4:
		bv = B00001111;
		break;

	case 5:
		bv = B00011111;
		break;

	case 6:
		bv = B00111111;
		break;

	case 7:
		bv = B01111111;
		break;

	case 8:
		bv = B11111111;
		break;

	}
	return bv;
}

inline uint8_t append(char* toBuf, uint8_t toOffset, uint8_t toSize, char* fromBuf) {
	uint8_t fromIdx = 0;
	uint8_t toIdx = toOffset;
	for (; toIdx < toSize; toIdx++) {
		char ch = fromBuf[fromIdx++];
		if (ch == '\0') {
			break;
		}
		toBuf[toIdx] = ch;
	}
	return toIdx;
}

inline boolean checkBit(uint8_t byte, uint8_t bit) {
	return byte & (1 << bit);
}

inline byte setBit(uint8_t byte, uint8_t bit) {
	return byte | 1 << bit;
}

inline void fbyte(uint8_t byte, char *buf) {
	uint8_t bit = 7;
	for (uint8_t idx = 0; idx < 8; idx++) {
		buf[idx] = checkBit(byte, bit) ? '1' : '0';
		bit--;
	}

	buf[8] = '\0';
}

/** Creates 2D array using continuous memory space. */
inline uint8_t** alloc2DArray8(uint8_t rows, uint8_t cols) {
	uint8_t **array = (uint8_t **) malloc(sizeof(uint8_t *) * rows);
	array[0] = (uint8_t*) malloc(sizeof(uint8_t) * rows * cols);

	for (uint8_t row = 1; row < rows; row++) {
		array[row] = (*array + cols * row);
	}
	return array;
}

inline uint8_t** init2DArray8(uint8_t rows, uint8_t cols) {
	uint8_t **array = alloc2DArray8(rows, cols);
	clean2DArray8(array, rows, cols);
	return array;
}

inline void delete2DArray8(uint8_t **array) {
	free(array[0]);
	free(array);
}

inline void cleanCharArray(char *array, uint8_t size) {
	for (uint8_t col = 0; col < size; col++) {
		array[col] = ' ';
	}
}

inline void clean2DArray8(uint8_t **array, uint8_t rowStart, uint8_t colStart, uint8_t rows, uint8_t cols) {
	for (uint8_t row = rowStart; row < rows; row++) {
		for (uint8_t col = colStart; col < cols; col++) {
			array[row][col] = 0;
		}
	}
}

inline void clean2DArray8(uint8_t **array, uint8_t rows, uint8_t cols) {
	clean2DArray8(array, 0, 0, rows, cols);
}

inline void pgmCopy(const __FlashStringHelper *ifsh, char *pgbuf, uint8_t bufSize) {
	PGM_P p = reinterpret_cast<PGM_P>(ifsh);
	unsigned char ch = 0;
	for(uint8_t pgbufIdx = 0; pgbufIdx < bufSize; pgbufIdx++) {
		ch = pgm_read_byte(p++);
		pgbuf[pgbufIdx] = ch;
		if(ch == 0) {
			break;
		}
	}
}

inline void shiftL(uint8_t array[], uint8_t size) {
	uint8_t sizel = size - 1;
	for (uint8_t idx = 0; idx < size; idx++) {
		uint8_t val = array[idx] << 1;

		// carry over bit from next byte
		if (idx < sizel) {
			if (array[idx + 1] & 0x80) { // set last bit to 1
				val |= 0x01;
			} else { // clear last bit
				val &= 0xFE;
			}
		}
		array[idx] = val;
	}
}

#endif /* ARD_DISP_UTIL_H */
