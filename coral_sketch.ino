
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 2
#define LED 13

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'
}


void loop() {

}

// strip.Color(255, 0, 0), 50);
// strip.setPixelColor(i, c);
// strip.show();

void setGridPixel(uint8_t x, uint8_t y, uint32_t color)
{
  // need to set up a 2d array for this one
}

void setStrandPixel(uint8_t i, uint32_t color)
{
  // just a 1d array
}

void setBranchPixel(uint8_t j, uint8_t i, uint32_t color)
{
  // this would be a jagged array, I guess?
}

void setDonutPixel(uint8_t i, uint32_t color)
{
  // a bunch of 1d arrays
}


uint32_t bleach(uint8_t health, uint32_t color)
{
  // need to revese engineer the color method from the strip
  // but let's assume we've handled that and we've got
  // so...
  byte r = 255;
  byte g = 128;
  byte b = 0;
  // just for example

  float health2 = health/255;

  byte r2 = 255 * (1 - health2) - r * health2;
  byte g2 = 255 * (1 - health2) - g * health2;
  byte b2 = 255 * (1 - health2) - b * health2;
  return strip.Color(r2, g2, b2);
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
