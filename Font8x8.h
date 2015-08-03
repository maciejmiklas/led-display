#ifndef FONT8x8_H_
#define FONT8x8_H_

#include "Arduino.h"

/**
 * Font size is set to 8x8 pixels. Each single font is stored in array containing 8 data bytes.
 * Each data byte represents one row (horizontal position) and bits within row are creating vertical dimension.
 * So data byte on position [0] gives top row, on position [8] bottom row and [3],[4] are in the middle.
 */
#define FONT_ASCI_SIZE 128

extern const uint8_t FONT_ASCI[FONT_ASCI_SIZE][8];

#endif /* FONT8x8_H_ */
