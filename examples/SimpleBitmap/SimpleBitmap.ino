#include <Display.h>

Display *disp;

/**
 * Orientation of LED Kits (8x8 LED matrix) on display that I've used for testing.
 * The numbers are indicating Select Slave line of MAX7219.
 * 48, 46, 49, 47, 45, 43, 41, 39
 * 36, 34, 32, 30, 28, 26, 24, 22
 * 37, 35, 33, 31, 29, 27, 25, 23
 */
ss_t **ss;

uint8_t ** data;

ss_t** createSS() {
  ss_t **ss = alloc2DArray8(3, 8);

  // first row
  ss[0][0] = 48;
  ss[0][1] = 46;
  ss[0][2] = 49;
  ss[0][3] = 47;
  ss[0][4] = 45;
  ss[0][5] = 43;
  ss[0][6] = 41;
  ss[0][7] = 39;

  // second row
  ss[1][0] = 36;
  ss[1][1] = 34;
  ss[1][2] = 32;
  ss[1][3] = 30;
  ss[1][4] = 28;
  ss[1][5] = 26;
  ss[1][6] = 24;
  ss[1][7] = 22;

  // third row
  ss[2][0] = 37;
  ss[2][1] = 35;
  ss[2][2] = 33;
  ss[2][3] = 31;
  ss[2][4] = 29;
  ss[2][5] = 27;
  ss[2][6] = 25;
  ss[2][7] = 23;

  return ss;
}

void setup() {
  util_setup();
  log_setup();
  
  ss = createSS();

  disp = new Display(8, 3, ss);
  disp->setup();

  data = alloc2DArray8(8, 2);
  data[0][0] = B01100001; data[0][1] = B10000000;
  data[1][0] = B01100001; data[1][1] = B10000000;
  data[2][0] = B01100001; data[2][1] = B10000000;
  data[3][0] = B01100001; data[3][1] = B10000000;
  data[4][0] = B01100001; data[4][1] = B10000000;
  data[5][0] = B00110011; data[5][1] = B00000000;
  data[6][0] = B00011110; data[6][1] = B00000000;
  data[7][0] = B00001100; data[7][1] = B00000000;

  disp->paint(27, 9, 8, 8, data);
}

void loop() {
  util_cycle();
  log_cycle();
  
   // Paint method updates only internal buffer, 
   // in order to send data to MAX chips you have to flush display.
  disp->flush();
  delay(100000);
}
