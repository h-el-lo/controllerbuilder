#ifndef RGB_H
#define RGB_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif


enum RGB_Mode {
  RGB_STATIC,
  RGB_PULSAR,
  RGB_MONTAGE,
  RGB_RAINBOW,
  RGB_PARTYTIME,
};

class RGBStrip {
private:
  Adafruit_NeoPixel _strip;
  uint8_t _brightness;
  RGB_Mode RGB_MODE = RGB_PARTYTIME;  // Animation mode of pixel
  uint32_t animationSpeed = 64;       // 0 - 100 %

  // HSV values of pixel
  uint16_t _hue = 10000;
  uint8_t _sat = 255;
  uint8_t _val = 255;

  // Synthage default colors
  uint8_t colors[12][3] = {
    { 51, 86, 255 },   // Color 1 - LightBlue // Done
    { 0, 0, 100 },     // Color 2 - Blue // Done
    { 101, 0, 205 },   // Color 3 - Indigo/Purple // Done
    { 80, 0, 87 },     // Color 4 - Violet // Done
    { 195, 0, 60 },    // Color 5 - Magenta // Done
    { 50, 3, 0 },      // Color 6 - Red // Done
    { 245, 65, 2 },    // Color 7 - Orange // Done
    { 255, 190, 0 },   // Color 8 - Yellow // Done
    { 180, 255, 0 },   // Color 9 - Lawn Green // Done
    { 120, 255, 0 },   // Color 10 - Green // Done
    { 72, 255, 51 },   // Color 11- Mint Green // Done
    { 75, 155, 214 },  // Color 12 - Cyan Done // Done
  };


public:
  RGBStrip(uint8_t LED_COUNT, uint8_t LED_PIN, uint8_t brightness)
    : _strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800), _brightness(brightness) {

    _strip.begin();
    _strip.setBrightness(_brightness);
    _strip.clear();

    // synthageSetupCode();
  };

  void setColor() {
    _brightness = map(analogRead(A6), 0, 1023, 0, 255);
    _strip.setBrightness(_brightness);

    _hue = map(analogRead(A0), 0, 1023, 0, 65536);
    _sat = map(analogRead(A1), 0, 1023, 255, 0);
    _val = map(analogRead(A2), 0, 1023, 0, 255);
    animationSpeed = map(analogRead(A3), 0, 1023, 64, 0);
    update();
  }

  // No animation, displays selected HSV color ONLY
  void inanimate() {
    uint32_t color = _strip.gamma32(_strip.ColorHSV(_hue, _sat, _val));  // hue -> RGB
    _strip.setPixelColor(0, color);
    _strip.show();
  }

  void montage() {
    // // This pattern is in four stages, increment, hold, decrement, hold.
    // // Animation variables (Very similar to Synthage)
    // uint16_t timeon = 600;  // milliseconds
    // uint8_t timeoff = 10;   // milliseconds
    // uint16_t steps = 40;    // frames per half-cycle
    // static uint32_t RGB_timer = 0;
    // uint16_t threshold = timeon / steps;
    // static uint16_t RGB_count = 0;
    // static uint8_t stage = 1;  // Begin animation at stage 1

    // if (stage == 1) {
    //   if (RGB_count < steps) {
    //     if (millis() - RGB_timer >= threshold) {
    //       _strip.setPixelColor(0, _strip.Color(((r * RGB_count) / steps), ((g * RGB_count) / steps), ((b * RGB_count) / steps)));
    //       _strip.show();
    //       RGB_timer = millis();
    //       RGB_count++;
    //     }
    //   }

    //   if (RGB_count == steps) {
    //     RGB_count = 0;
    //     _strip.setPixelColor(0, _strip.Color(r, g, b));
    //     _strip.show();
    //     RGB_timer = millis();  // Reset the RGB_timer variable
    //     stage = 2;
    //   }


    // } else if (stage == 2) {
    //   if (millis() - RGB_timer >= timeoff) {
    //     RGB_timer = millis();  // Reset the RGB_timer variable
    //     stage = 3;
    //   }

    // } else if (stage == 3) {

    //   if (RGB_count < steps) {
    //     if (millis() - RGB_timer >= threshold) {
    //       _strip.setPixelColor(0, _strip.Color(((r * (steps - RGB_count)) / steps), ((g * (steps - RGB_count)) / steps), ((b * (steps - RGB_count)) / steps)));
    //       _strip.show();
    //       RGB_timer = millis();
    //       RGB_count++;
    //     }
    //   }

    //   if (RGB_count == steps) {
    //     RGB_count = 0;
    //     _strip.setPixelColor(0, _strip.Color(0, 0, 0));
    //     _strip.show();
    //     RGB_timer = millis();  // Reset the RGB_timer variable
    //     stage = 4;
    //   }

    // } else if (stage == 4) {
    //   if (millis() - RGB_timer >= timeoff) {
    //     RGB_timer = millis();  // Reset the RGB_timer variable
    //     stage = 1;
    //   }
    // }
  }

  // Rainbow cycle along whole_strip. Pass delay time (in ms) between frames.
  // Modified from Adafruit Neopixel's library example "strandtest"
  // Now runs without code blocking, without a for loop and the delay function.
  void rainbow() {
    uint8_t wait = map(animationSpeed, 0, 64, 0, 12);
    static uint16_t firstPixelHue = 0;  // any additions past 65536 will overflow
    //                                     and rollback to newNum % 65536 per size limitation of uint16_t
    static uint32_t lastUpdatedTime = millis();

    // Hue of first pixel runs a complete loop through the color wheel.
    // Color wheel has a range of 65536 but it's OK if we roll over, so
    // just count from 0 to 65536. Adding 256 to firstPixelHue each time
    // means we'll make 65536/256 = 256 passes through this loop:

    if ((millis() - lastUpdatedTime) < wait) return;
    _strip.rainbow(firstPixelHue);
    _strip.show();  // Update_strip with new contents
    firstPixelHue += 256;
    lastUpdatedTime = millis();
  }

  // Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
  void partyTime() {
    uint16_t wait = map(animationSpeed, 0, 100, 0, 500);
    static uint16_t pixelHue = _hue;  // First pixel starts at red (hue 0)
    uint8_t frames = 16;
    static uint32_t lastUpdated = 0;
    static bool pixelIsOn = 0;

    if ((millis() - lastUpdated) < wait) return;

    if (pixelIsOn) {
      _strip.clear();
      _strip.show();
    } else {
      uint32_t color = _strip.gamma32(_strip.ColorHSV(pixelHue, _sat, _val));  // hue -> RGB
      _strip.setPixelColor(0, color);
      _strip.show();  // Update_strip with new contents
      pixelHue += 65536 / frames;
    }

    lastUpdated = millis();
    pixelIsOn = !pixelIsOn;  // Invert pixelIsOn variable
  }


  void update() {
    switch (RGB_MODE) {
      case RGB_STATIC:
        inanimate();
        break;
      case RGB_PULSAR:
        break;
      case RGB_MONTAGE:
        montage();
        break;
      case RGB_RAINBOW:
        rainbow();
        break;
      case RGB_PARTYTIME:
        partyTime();
        break;
      default:
        inanimate();
        break;
    }
  }
};

extern RGBStrip MY_PIXEL;

#endif