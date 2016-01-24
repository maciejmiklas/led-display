This project contains driver for 8x8 LED Moduls controlled via MAX722xx. It allows you to build display of custom size that is only limited by the hardware itself. Vertical and horizontal size can contain up to 256 modules, but before reaching this limit you would run out of Slave Select lines for controlling MAX chips, or you would be limited by amount of RAM. The fact is: you can control reasonable amount of MAX chips and build display of custom size ;)

I've tested the whole idea on display that consist of 8 LED Modules in horizontal and 3 in vertical position. This gives us in total 24 modules which are containing 1536 LEDs (8*8 * 3*8).

# Hardware
First let's start with the controller. Actually any Arduino will work, I've used Mega due to large number of digital output pins. You could also use shift register and alter way of addressing Select Slave lines in *Display::send(...)*.

You will need extra power supply for driving LEDs - assuming that you are going to use more than one LED Matrix.

## Driving single LED Matrix
I've used 788BS LED Matrix, this is the one with common anode. If you have one with common cathode, you will have to rewire pins. On the schematic below you can see the wiring of single LED Display, MAX and Arduino. 
![Select Slave pins](/doc/fritzing/disp_ss.jpg)
![Single LED Module](/doc/fritzing/LEDModule.jpg)
Bot schematics above are equivalent - the just illustrate different perspective.

## Connecting all LED Matix together
![Test LED Display](/doc/fritzing/TestLEDDisplay.jpg)
All MAX722xx chips share common MOSI and SCK lines, MISO is not used, each chip requires separate Slave Select line. 
The position of LED Marix on the schematic above directly corresponds to their location on the accrual display, that I've used for testing. 

#Software
## Setting things up
The main class of your interest will be the *Display.h* - it's responsible for setup of MAX chips and provides API for painting. 

Before we start painting it's necessary to set thing up. Code below creates 2D array with Select Slave lines and initializes display. 

```
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

  
  // Test display consist of 8x3 LED Kits (3 rows, each one 8 LED Kits)
  disp = new Display(8, 3, ss);
  disp->setup();
}
```

There is one more method worth mentioning: *log_setup()*. Whole project has quiet precise logger - so that you can see what is actually happening. By default it's disabled, in order to enable it check out its documentation: https://github.com/maciejmiklas/ArdLog

## Painting on the display
Display consists of a few kits, but form API perspective those kits are connected together into one continuous canvas. You can place on this canvas bitmap on any position given by such coordinates:

```
 (0,0) -----------------------------> (x)
      |
      |
      |
      |
      |
      |
      |                            (x max, y max)
      v (y)
```

The paint method has following syntax: *paint(pixel_t x, pixel_t y, pixel_t width, pixel_t height, uint8_t **data)*. It allows you to paint a bitmap on given coordinates with limited width and height. So you can for example paint a bitmap on (3,4) that has 25x3 pixels. It might be larger than a actual display size - in this case it will get trimmed.
This is obvious and simple, but there is one catch - you have to provide right *data*. This is 2D array, where first dimension indicates vertical and second  horizontal position on the display. Technically speaking *data* is flat array of pointers and each pointer points to array that represents one horizontal line on the display.

Moving over first dimension of *data* traverses over lines of the display. The second dimension of *data* represents horizontal pixels on display, but there one optimization in place. Since our display consist of simple LEDs they can be either in on or off state, each pixel is not being represented by one byte, but by one bit. In order to cover 8 pixels in horizontal position we need one byte (to cover 24 pixels we need 3 bytes).

For example to fully cover display consisting of 8x3 LED kits (one used in our examples) we would need *data[3][8]*. Usually you will take array small enough to fit your bitmap and not one that will cover up whole display.

The *paint(...)* method updates internal buffer, in order to send content of this buffer to MAX chips you have to call *flush()*. The idea behind is to give you possibility to display few bitmaps on the display and after that paint the result. You can program few independent routines, that will update different part of the display and flush all changes at once.

Communication with MAX chips is not very fast and sending content of the whole display with every *flush()* is time consuming. You might be able to speed up this process by enabling double buffering (set *DEOUBLE_BUFFER* in *Display.h* to true). In this case *flush()* method will send only bytes that have changed, so you can call *flush()* with every loop and do not have to worry about loosing performance. The only drawback is increased usage of RAM: we are creating 2D array that allocates 8 bytes per each LED Kit plus few pointers that are usually required to maintain arrays. However 2D arrays in this project are well optimized (hopefully), because in order to create dynamic 2D array, we are creating actually 2 arrays with calculated offset (see: *alloc2DArray8(....)* in *Util.h*).

# Examples
# Requires Libs
Examples are using ArdLogger, so you have to import this lib into our Arduino IDE. Here are instructions: https://github.com/maciejmiklas/ArdLog

## Simple Bitmap
<img src="/doc/img/disp_v.jpg" width="300px"/>


Here is the Arduino sketch: [SimpleBitmat](/examples/SimpleBitmat/SimpleBitmat.ino), now let's discuss it:

First we have to create data that can hold out bitmap - it will have 3x2 bytes. This gives us up to 3 lines and 16 horizontal pixels. But the size of out bitmap is 
9x3 pixels and this will be also size of the painted rectangle. It should be as small as possible, so that you can place another bitmap next to it. The display will obviously only paint the rectangle and not whole *data* array.

```
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

  disp->paint(27, 9, 9, 8, data);
}

void loop() {
  util_cycle();
  log_cycle();
  
  // Paint method updates only internal buffer, in order to send data to MAX chips you have to flush display. 
  disp->flush();
  
  delay(100000);
}
```

## Static Text
<img src="/doc/img/disp_static.jpg" width="300px"/>

Here you can find Arduino sketch containing whole example: [StaticText](/examples/StaticText). 

Your sketch needs setup method as we've already seen above. So we are assuming that you have already created *Display* and now you would like to show simple text. For this you should use class class *StaticText8x8*, it could look like this one:

```
StaticText8x8 *sta1;
StaticText8x8 *sta2;

void setup() {
  util_setup();
  log_setup();
  
  ss = createSS();

  disp = new Display(8, 3, ss);
  disp->setup();

  sta1 = new StaticText8x8(disp, 64);
  sta1->box(14, 2, "Hello");

  sta2 = new StaticText8x8(disp, 64);
  sta2->box(5, 15, "World !");
}

void loop() {
  util_cycle();
  log_cycle();
  disp->flush();
  
  delay(100000);
}
```
As a result we would display text "Hello !" on (3,10). Font is defined in class: [Font8x8](https://github.com/maciejmiklas/LEDDisplay/blob/master/Font8x8.cpp), each character has 8x8 pixels.

## Single Scrolling Text

## Scrolling Text Mixed
![](/doc/img/disp_scroll.jpg)
This sketch contains whole example: [ScrollingText](/examples/ScrollingText). Below we will discuss simplified version, so that you can get the idea.

As in the previous example - we are assuming, that you have proper instance of *Display*, but this time you would like to show text scrolling within a box. The class *ScrollingText8x8* will handle it:
```
ScrollingText8x8 *message;
const char *textMessage;
Display *disp;

void setup() {
  util_setup();
  log_setup();
  
  ss = createSS();

  disp = new Display(8, 3, ss);
  disp->setup();

  message = new ScrollingText8x8(disp, 48, 50, 5);
  message->init();
  textMessage = "This is an example of multiple scorlling areas ;)";
  message->scroll(8, 8, ScrollingText8x8::LOOP, textMessage);
}

void loop() {
  util_cycle();
  log_cycle();

  message->cycle();
  
  disp->flush();
}
```

In the setup method we have created instance of *ScrollingText8x8* and also scrolling area that will be displayed in the left corner of the display. Frames will be animated every 200ms. Once scrolling of given text is done, it will start from the beginning. 

It is important to NOT define any delays in *loop()* because *scr->cycle()* has to be called as offen as possible in order to get smooth animation. The implementation is based on State Machine Pattern - so it uses CPU only when there is something to be done, there are not delays inside.



