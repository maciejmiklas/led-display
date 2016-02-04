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
 #ifndef LD_Font8x8_h
#define LD_Font8x8_h

#include "Arduino.h"
#include "ArdLog.h"

/**
 * Font size is set to 8x8 pixels. Each single font is stored in array containing 8 data bytes.
 * Each data byte represents one row (horizontal position) and bits within row are creating vertical dimension.
 * So data byte on position [0] gives top row, on position [8] bottom row and [3],[4] are in the middle.
 */
const uint8_t FONT8_SIZE = 128;
const uint8_t FONT8_WIDTH = 8;
const uint8_t FONT8_HEIGHT = 8;

PROGMEM extern const uint8_t FONT8[FONT8_SIZE][FONT8_HEIGHT];

void font8x8_copy(uint8_t **data, uint8_t dataIdx, uint8_t fontIdx);

#endif /* LD_Font8x8_h */
