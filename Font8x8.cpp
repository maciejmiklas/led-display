#include "Font8x8.h"

PROGMEM const uint8_t FONT8[FONT8_SIZE][FONT8_HEIGHT] = {
// Based on: https://github.com/dhepper/font8x8/blob/master/font8x8_basic.h
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // 0 - U+0000 (nul)
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 1 - U+0001
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 2 - U+0002
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 3 - U+0003
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 4 - U+0004
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 5 - U+0005
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 6 - U+0006
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 7 - U+0007
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 8 - U+0008
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 9 - U+0009
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 10 - U+000A
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 11 - U+000B
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 12 - U+000C
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 13 - U+000D
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 14 - U+000E
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 15 - U+000F
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 16 - U+0010
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 17 - U+0011
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 18 - U+0012
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 19 - U+0013
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 20 - U+0014
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 21 - U+0015
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 22 - U+0016
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 23 - U+0017
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 24 - U+0018
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 25 - U+0019
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 26 - U+001A
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 27 - U+001B
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 28 - U+001C
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 29 - U+001D
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 30 - U+001E
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 31 - U+001F
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 32 - U+0020 (space)
		{ 0x18, 0x3c, 0x3c, 0x18, 0x18, 0x00, 0x18, 0x00 },   // 33 - U+0021 (!)
		{ 0x6c, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 34 - U+0022 (")
		{ 0x6c, 0x6c, 0xfe, 0x6c, 0xfe, 0x6c, 0x6c, 0x00 },   // 35 - U+0023 (#)
		{ 0x30, 0x7c, 0xc0, 0x78, 0x0c, 0xf8, 0x30, 0x00 },   // 36 - U+0024 ($)
		{ 0x00, 0xc6, 0xcc, 0x18, 0x30, 0x66, 0xc6, 0x00 },   // 37 - U+0025 (%)
		{ 0x38, 0x6c, 0x38, 0x76, 0xdc, 0xcc, 0x76, 0x00 },   // 38 - U+0026 (&)
		{ 0x60, 0x60, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 39 - U+0027 (')
		{ 0x18, 0x30, 0x60, 0x60, 0x60, 0x30, 0x18, 0x00 },   // 40 - U+0028 (()
		{ 0x60, 0x30, 0x18, 0x18, 0x18, 0x30, 0x60, 0x00 },   // 41 - U+0029 ())
		{ 0x00, 0x66, 0x3c, 0xff, 0x3c, 0x66, 0x00, 0x00 },   // 42 - U+002A (*)
		{ 0x00, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x00, 0x00 },   // 43 - U+002B (+)
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x60 },   // 44 - U+002C (,)
		{ 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00 },   // 45 - U+002D (-)
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00 },   // 46 - U+002E (.)
		{ 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x80, 0x00 },   // 47 - U+002F (/)
		{ 0x7c, 0xc6, 0xce, 0xde, 0xf6, 0xe6, 0x7c, 0x00 },   // 48 - U+0030 (0)
		{ 0x30, 0x70, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x00 },   // 49 - U+0031 (1)
		{ 0x78, 0xcc, 0x0c, 0x38, 0x60, 0xcc, 0xfc, 0x00 },   // 50 - U+0032 (2)
		{ 0x78, 0xcc, 0x0c, 0x38, 0x0c, 0xcc, 0x78, 0x00 },   // 51 - U+0033 (3)
		{ 0x1c, 0x3c, 0x6c, 0xcc, 0xfe, 0x0c, 0x1e, 0x00 },   // 52 - U+0034 (4)
		{ 0xfc, 0xc0, 0xf8, 0x0c, 0x0c, 0xcc, 0x78, 0x00 },   // 53 - U+0035 (5)
		{ 0x38, 0x60, 0xc0, 0xf8, 0xcc, 0xcc, 0x78, 0x00 },   // 54 - U+0036 (6)
		{ 0xfc, 0xcc, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x00 },   // 55 - U+0037 (7)
		{ 0x78, 0xcc, 0xcc, 0x78, 0xcc, 0xcc, 0x78, 0x00 },   // 56 - U+0038 (8)
		{ 0x78, 0xcc, 0xcc, 0x7c, 0x0c, 0x18, 0x70, 0x00 },   // 57 - U+0039 (9)
		{ 0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x00 },   // 58 - U+003A (:)
		{ 0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x60 },   // 59 - U+003B (//)
		{ 0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x00 },   // 60 - U+003C (<)
		{ 0x00, 0x00, 0xfc, 0x00, 0x00, 0xfc, 0x00, 0x00 },   // 61 - U+003D (=)
		{ 0x60, 0x30, 0x18, 0x0c, 0x18, 0x30, 0x60, 0x00 },   // 62 - U+003E (>)
		{ 0x78, 0xcc, 0x0c, 0x18, 0x30, 0x00, 0x30, 0x00 },   // 63 - U+003F (?)
		{ 0x7c, 0xc6, 0xde, 0xde, 0xde, 0xc0, 0x78, 0x00 },   // 64 - U+0040 (@)
		{ 0x30, 0x78, 0xcc, 0xcc, 0xfc, 0xcc, 0xcc, 0x00 },   // 65 - U+0041 (A)
		{ 0xfc, 0x66, 0x66, 0x7c, 0x66, 0x66, 0xfc, 0x00 },   // 66 - U+0042 (B)
		{ 0x3c, 0x66, 0xc0, 0xc0, 0xc0, 0x66, 0x3c, 0x00 },   // 67 - U+0043 (C)
		{ 0xf8, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0xf8, 0x00 },   // 68 - U+0044 (D)
		{ 0xfe, 0x62, 0x68, 0x78, 0x68, 0x62, 0xfe, 0x00 },   // 69 - U+0045 (E)
		{ 0xfe, 0x62, 0x68, 0x78, 0x68, 0x60, 0xf0, 0x00 },   // 70 - U+0046 (F)
		{ 0x3c, 0x66, 0xc0, 0xc0, 0xce, 0x66, 0x3e, 0x00 },   // 71 - U+0047 (G)
		{ 0xcc, 0xcc, 0xcc, 0xfc, 0xcc, 0xcc, 0xcc, 0x00 },   // 72 - U+0048 (H)
		{ 0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00 },   // 73 - U+0049 (I)
		{ 0x1e, 0x0c, 0x0c, 0x0c, 0xcc, 0xcc, 0x78, 0x00 },   // 74 - U+004A (J)
		{ 0xe6, 0x66, 0x6c, 0x78, 0x6c, 0x66, 0xe6, 0x00 },   // 75 - U+004B (K)
		{ 0xf0, 0x60, 0x60, 0x60, 0x62, 0x66, 0xfe, 0x00 },   // 76 - U+004C (L)
		{ 0xc6, 0xee, 0xfe, 0xfe, 0xd6, 0xc6, 0xc6, 0x00 },   // 77 - U+004D (M)
		{ 0xc6, 0xe6, 0xf6, 0xde, 0xce, 0xc6, 0xc6, 0x00 },   // 78 - U+004E (N)
		{ 0x38, 0x6c, 0xc6, 0xc6, 0xc6, 0x6c, 0x38, 0x00 },   // 79 - U+004F (O)
		{ 0xfc, 0x66, 0x66, 0x7c, 0x60, 0x60, 0xf0, 0x00 },   // 80 - U+0050 (P)
		{ 0x78, 0xcc, 0xcc, 0xcc, 0xdc, 0x78, 0x1c, 0x00 },   // 81 - U+0051 (Q)
		{ 0xfc, 0x66, 0x66, 0x7c, 0x6c, 0x66, 0xe6, 0x00 },   // 82 - U+0052 (R)
		{ 0x78, 0xcc, 0xe0, 0x70, 0x1c, 0xcc, 0x78, 0x00 },   // 83 - U+0053 (S)
		{ 0xfc, 0xb4, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00 },   // 84 - U+0054 (T)
		{ 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xfc, 0x00 },   // 85 - U+0055 (U)
		{ 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x78, 0x30, 0x00 },   // 86 - U+0056 (V)
		{ 0xc6, 0xc6, 0xc6, 0xd6, 0xfe, 0xee, 0xc6, 0x00 },   // 87 - U+0057 (W)
		{ 0xc6, 0xc6, 0x6c, 0x38, 0x38, 0x6c, 0xc6, 0x00 },   // 88 - U+0058 (X)
		{ 0xcc, 0xcc, 0xcc, 0x78, 0x30, 0x30, 0x78, 0x00 },   // 89 - U+0059 (Y)
		{ 0xfe, 0xc6, 0x8c, 0x18, 0x32, 0x66, 0xfe, 0x00 },   // 90 - U+005A (Z)
		{ 0x78, 0x60, 0x60, 0x60, 0x60, 0x60, 0x78, 0x00 },   // 91 - U+005B ([)
		{ 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x02, 0x00 },   // 92 - U+005C (\)
		{ 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00 },   // 93 - U+005D (])
		{ 0x10, 0x38, 0x6c, 0xc6, 0x00, 0x00, 0x00, 0x00 },   // 94 - U+005E (^)
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff },   // 95 - U+005F (_)
		{ 0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 96 - U+0060 (`)
		{ 0x00, 0x00, 0x78, 0x0c, 0x7c, 0xcc, 0x76, 0x00 },   // 97 - U+0061 (a)
		{ 0xe0, 0x60, 0x60, 0x7c, 0x66, 0x66, 0xdc, 0x00 },   // 98 - U+0062 (b)
		{ 0x00, 0x00, 0x78, 0xcc, 0xc0, 0xcc, 0x78, 0x00 },   // 99 - U+0063 (c)
		{ 0x1c, 0x0c, 0x0c, 0x7c, 0xcc, 0xcc, 0x76, 0x00 },   // 100 - U+0064 (d)
		{ 0x00, 0x00, 0x78, 0xcc, 0xfc, 0xc0, 0x78, 0x00 },   // 101 - U+0065 (e)
		{ 0x38, 0x6c, 0x60, 0xf0, 0x60, 0x60, 0xf0, 0x00 },   // 102 - U+0066 (f)
		{ 0x00, 0x00, 0x76, 0xcc, 0xcc, 0x7c, 0x0c, 0xf8 },   // 103 - U+0067 (g)
		{ 0xe0, 0x60, 0x6c, 0x76, 0x66, 0x66, 0xe6, 0x00 },   // 104 - U+0068 (h)
		{ 0x30, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00 },   // 105 - U+0069 (i)
		{ 0x0c, 0x00, 0x0c, 0x0c, 0x0c, 0xcc, 0xcc, 0x78 },   // 106 - U+006A (j)
		{ 0xe0, 0x60, 0x66, 0x6c, 0x78, 0x6c, 0xe6, 0x00 },   // 107 - U+006B (k)
		{ 0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00 },   // 108 - U+006C (l)
		{ 0x00, 0x00, 0xcc, 0xfe, 0xfe, 0xd6, 0xc6, 0x00 },   // 109 - U+006D (m)
		{ 0x00, 0x00, 0xf8, 0xcc, 0xcc, 0xcc, 0xcc, 0x00 },   // 110 - U+006E (n)
		{ 0x00, 0x00, 0x78, 0xcc, 0xcc, 0xcc, 0x78, 0x00 },   // 111 - U+006F (o)
		{ 0x00, 0x00, 0xdc, 0x66, 0x66, 0x7c, 0x60, 0xf0 },   // 112 - U+0070 (p)
		{ 0x00, 0x00, 0x76, 0xcc, 0xcc, 0x7c, 0x0c, 0x1e },   // 113 - U+0071 (q)
		{ 0x00, 0x00, 0xdc, 0x76, 0x66, 0x60, 0xf0, 0x00 },   // 114 - U+0072 (r)
		{ 0x00, 0x00, 0x7c, 0xc0, 0x78, 0x0c, 0xf8, 0x00 },   // 115 - U+0073 (s)
		{ 0x10, 0x30, 0x7c, 0x30, 0x30, 0x34, 0x18, 0x00 },   // 116 - U+0074 (t)
		{ 0x00, 0x00, 0xcc, 0xcc, 0xcc, 0xcc, 0x76, 0x00 },   // 117 - U+0075 (u)
		{ 0x00, 0x00, 0xcc, 0xcc, 0xcc, 0x78, 0x30, 0x00 },   // 118 - U+0076 (v)
		{ 0x00, 0x00, 0xc6, 0xd6, 0xfe, 0xfe, 0x6c, 0x00 },   // 119 - U+0077 (w)
		{ 0x00, 0x00, 0xc6, 0x6c, 0x38, 0x6c, 0xc6, 0x00 },   // 120 - U+0078 (x)
		{ 0x00, 0x00, 0xcc, 0xcc, 0xcc, 0x7c, 0x0c, 0xf8 },   // 121 - U+0079 (y)
		{ 0x00, 0x00, 0x7E, 0x4C, 0x18, 0x32, 0x7E, 0x00 },   // 122 - U+007A (z)
		{ 0x30, 0x30, 0xe0, 0x30, 0x30, 0x1c, 0x00, 0x68 },   // 123 - U+007B ({)
		{ 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00, 0x68 },   // 124 - U+007C (|)
		{ 0x30, 0x30, 0x1c, 0x30, 0x30, 0xe0, 0x00, 0x6e },   // 125 - U+007D (})
		{ 0xdc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68 },   // 126 - U+007E (~)
		{ 0x67, 0xfb, 0xf8, 0xed, 0xb7, 0xf7, 0xb5, 0xfb }    // 127 - U+007F
};

void font8x8_copy(uint8_t **data, uint8_t dataIdx, uint8_t fontIdx) {
#if LOG
	log(F("Copy font: %d"), fontIdx);
#endif

	for (uint8_t row = 0; row < FONT8_HEIGHT; row++) {
		data[row][dataIdx] = pgm_read_byte(&FONT8[fontIdx][row]);
#if LOG
		log(F(" -- data[%d][%d] = 0x%02x"), row, dataIdx, data[row][dataIdx]);
#endif
	}
}
