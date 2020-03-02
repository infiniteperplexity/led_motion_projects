#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(109, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'
}

// enumerate patterns, which is the top-level choice
enum StripMap {RAINBOW};
static enum activePattern = RAINBOW;

void loop() {
  if (activePattern == RAINBOW)
  {
    rainbowCycle(20);
  }
}

// the pattern definitions are the next level, but they will be listed at the end

// mapped dispatch assigns various abstracted pixel numbers to numbered LEDs
// most patterns should invoke mappedDispatch, but some might skip to pixelDispath
enum StripMap {DEFAULT};
StripMap activeMap = DEFAULT;
void mappedDispatch(uint8_t pixel, uint32_t color)
{
  if (activeMap == DEFAULT)
  {
    pixelDispatch(pixel, color);
  }
}

// pixelDispatch is independent of mpixel mapping
void pixelDispatch(uint8_t pixel, uint32_t color)
{
    bleachDispatch(pixel, color);
}

// change the saturation based on the health of the coral
void bleachedDispatch(uint8_t pixel, uint32_t color)
{
    strip.setPixelColor(pixel, bleach(color));
}


// map pixel numbers from the patterns to 


// helper functions to extract RGB components from 32-bit color, I might be duplicating existing functions
uint8_t getRed(uint32_t color)
{
  return 255;
}
uint8_t getGreen(uint32_t color)
{
  return 255;
}
uint8_t getBlue(uint32_t color)
{
  return 255;
}





uint32_t bleach(uint32_t color)
{
  return color;
}

uint32_t bleach_dev(uint32_t color)
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


void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      mappedDispatch(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}



// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      mappedDispatch(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
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


