// simple project using Arduino UNO, NeoPixel ring and Rotary Encoder to fill the ring with colors

// created by upir, 2024
// youtube channel: https://www.youtube.com/upir_upir

// YOUTUBE VIDEO: https://youtu.be/JK3KePYrkYY
// Source Files: https://github.com/upiir/arduino_neopixel_rings
// Wokwi sketch: https://wokwi.com/projects/392984672413834241

// Links from the video:
// Wokwi starting sketch: https://wokwi.com/projects/374907938100932609
// Adafruit NeoPixel guide: https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use
// NeoPixel ring(s): https://s.click.aliexpress.com/e/_DdIyIgl
// Rotary Encoder Module: https://s.click.aliexpress.com/e/_DETznkv
// Solid Aluminium Knob: https://s.click.aliexpress.com/e/_DFPuloh
// Breadboard wires: https://s.click.aliexpress.com/e/_Dkbngin
// Arduino UNO R3: https://s.click.aliexpress.com/e/_AXDw1h
// Arduino breadboard prototyping shield: https://s.click.aliexpress.com/e/_DlxEfPX

// Related videos:
// Pimp My Potentiometer! - https://youtu.be/tHL4RYGSvg4
// Pimp My Potentiometer (again) - https://youtu.be/sE3LSYoCqLQ
// Pimp my Potentiometer -CHEAPER- - 	https://youtu.be/Bot865qmdsM
// Your 1st CNC-ed Part - https://youtu.be/EB8PISr4m4g
// My First CNCed part --EVER-- - https://youtu.be/e2UNsZPjHkk
// My First PCB --EVER-- - https://youtu.be/6cqvTHCuDto
// DIY Volume Control - https://youtu.be/jvHRfsgw4l8
// Knob over OLED Display - https://youtu.be/SmbcNx7tbX8



#include <Adafruit_NeoPixel.h> // library for NeoPixels
#include <Encoder.h> // library for rotary encoder

#define PIN_NEO_PIXEL 6  // Arduino pin that connects to NeoPixel
#define NUM_PIXELS 16 // The number of LEDs (pixels) on NeoPixel
#define ENCODER_BTN 4 // rotary encoder button pin

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800); // set the NeoPixel ring initialization
Encoder myEnc(2, 3); // initialize the rotary encoder

byte color_blue_RGB[3] = {0, 0, 255}; // blue color in RGB color space
byte color_red_RGB[3] = {255, 0, 0}; // red color in RGB color space
byte color_blend_RGB[3]; // variable to store blended RGB color

byte color_blue_HSV[3] = {170, 255, 255}; // blue color in HSV color space
byte color_red_HSV[3] = {0, 255, 255}; // red color in HSV color space
byte color_blend_HSV[3]; // variable to store blended HSV color


int encoder_value_old = 0; // previous value of rotary encoder
int encoder_value_new = 0; // current value of rotary encoder
int percentage_value = 50; // percentage value to fill the ring: 0 - 100%


int mode = 0; // 0 = white, 1 = blue to red RGB, 2 = blue to red HSV


// function to blend between two colors
void blend_colors (byte color_start[3], byte color_end[3], byte color_blended[3], float percentage /*0-100*/) {
  for (int i = 0; i < 3; i ++) {
    // linear interpolation between two values
    color_blended[i] = round((float)color_start[i] + (((float)color_end[i] - (float)color_start[i]) * (percentage / 100.0)));
  }
}

void setup() {
  NeoPixel.begin();  // initialize the NeoPixel strip object
  pinMode(ENCODER_BTN, INPUT_PULLUP); // set pin for rotary encoder button
}

void loop() {

  // read the new value of rotary encoder
  encoder_value_new = myEnc.read(); // read the encoder value
  if (encoder_value_new != encoder_value_old) { // if the old value does not equal the new value
    percentage_value = constrain(percentage_value - (encoder_value_new - encoder_value_old), 0, 100); // set the percentage value between 0-100%
    encoder_value_old = encoder_value_new; // update the old encoder value
  }


  if (digitalRead(ENCODER_BTN) == LOW) {
    // switch the mode
    mode++;
    if (mode > 2) {
      mode = 0;
    }
    delay(500); // add a small delay
  }

  NeoPixel.clear();  // set all pixel colors to 'off'. It only takes effect if pixels.show() is called

  // go over every single pixel on the neopixel ring
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {  // for each pixel
    if ((pixel * (100 / (NUM_PIXELS - 1))) < percentage_value) { // fill pixels based on the rotary encoder value
      if (mode == 0) { // draw white pixels
        // set pixels to WHITE color
        NeoPixel.setPixelColor(pixel, NeoPixel.Color(255, 255, 255));  // set all pixels to white color

      } else if (mode == 1) { // blend between blue and red in RGB
        // blend two colors in RGB color space
        blend_colors(color_blue_RGB, color_red_RGB, color_blend_RGB, pixel * (100.0 / (NUM_PIXELS - 1)));
        // set the pixel to the blended color
        NeoPixel.setPixelColor(pixel, NeoPixel.Color(color_blend_RGB[0], color_blend_RGB[1], color_blend_RGB[2]));

      } else if (mode == 2) { // blend between blue and red in HSV
        // blend two colors in HSV color space
        blend_colors(color_blue_HSV, color_red_HSV, color_blend_HSV, pixel * (100.0 / (NUM_PIXELS - 1)));
        // set the pixel to the blended color
        NeoPixel.setPixelColor(pixel, NeoPixel.ColorHSV((unsigned int)color_blend_HSV[0] * 256, color_blend_HSV[1], color_blend_HSV[2]));

      }
    }
  }

  //NeoPixel.setBrightness(50);  // don´t forget to uncomment this line for real Arduino to make it less bright
  NeoPixel.show(); // show all the set pixels on neopixel ring
}
