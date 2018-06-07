#include <Display.h>
#include <ScrollingText8x8.h>

Display *disp;

ScrollingText8x8 *message;
const char *textMessage;

/**
 * Orientation of LED Kits (8x8 LED matrix) on display that I've used for testing.
 * The numbers are indicating Select Slave line of MAX7219.
 * 48, 46, 49, 47, 45, 43, 41, 39
 * 36, 34, 32, 30, 28, 26, 24, 22
 * 37, 35, 33, 31, 29, 27, 25, 23
 */
ss_t **ss;

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
  dutil_setup();
  log_setup();
  
  ss = createSS();

  disp = new Display(8, 3, ss);
  disp->setup();

  message = new ScrollingText8x8(disp, 48, 50, 5);
  message->init();
  textMessage = "This is an example of multiple scrolling areas ;)";
  message->scroll(8, 8, ScrollingText8x8::LOOP, textMessage);
}

void loop() {
  dutil_cycle();
  log_cycle();

  message->cycle();
  
  disp->flush();
}
