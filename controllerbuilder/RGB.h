#include <stdint.h>
#ifndef RGB_H
#define RGB_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "ControllerModes.h"
#include "MIDIHelper.h"

#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif


enum RGB_Mode : uint8_t {
  RGB_STATIC,
  // RGB_PULSAR,
  RGB_MONTAGE,
  RGB_RAINBOWMONTAGE,
  RGB_RAINBOW,
  RGB_PARTYTIME,
  RGB_MODES_NUM,
};


class RGBStrip {
private:

  Adafruit_NeoPixel _strip;  // Neopixel object
  uint8_t _brightness;

  // Default HSV values of pixel
  uint16_t _hue = 10000;
  uint8_t _sat = 255;
  uint8_t _val = 255;

  uint32_t animationSpeed = 20;  // 0 - 64
  RGB_Mode RGB_MODE = RGB_MONTAGE;
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
  };

  // Getters
  RGB_Mode getMode() {
    return RGB_MODE;
  }

  uint16_t getHue() {
    return _hue;
  }

  uint8_t getSat() {
    return _sat;
  }

  uint8_t getVal() {
    return _val;
  }

  uint8_t getBrightness() {
    return _brightness;
  }

  void setMode(RGB_Mode rgb_mode) {
    RGBStrip::RGB_MODE = rgb_mode;
  }

  void setColor(uint16_t hue, uint8_t sat, uint8_t val, uint8_t brightness) {
    _hue = hue;
    _sat = sat;
    _val = val;
    _brightness = brightness;
    _strip.setBrightness(_brightness);
  }

  void nextColorMode() {
    int8_t mode = static_cast<uint8_t>(RGB_MODE);
    mode += 1;
    mode = ((mode % RGB_MODES_NUM) + RGB_MODES_NUM) % RGB_MODES_NUM;
    RGB_MODE = static_cast<RGB_Mode>(mode);
  }

  void previousColorMode() {
    int8_t mode = static_cast<uint8_t>(RGB_MODE);
    mode -= 1;
    mode = ((mode % RGB_MODES_NUM) + RGB_MODES_NUM) % RGB_MODES_NUM;
    RGB_MODE = static_cast<RGB_Mode>(mode);
  }

  void editColorAndAnimation() {
    _brightness = map(analogRead(A6), 0, 1023, 0, 255);
    _strip.setBrightness(_brightness);

    _hue = map(analogRead(A0), 0, 1023, 0, 65536);
    _sat = map(analogRead(A1), 0, 1023, 255, 0);
    _val = map(analogRead(A2), 0, 1023, 0, 255);
    animationSpeed = map(analogRead(A3), 0, 1023, 64, 0);
    update();
  }

  void runningModeStartAnimation() {
    _strip.setBrightness(150);
    for (int i = 0; i < 3; i++) {                                         // For each pixel in strip...
      uint32_t color = _strip.gamma32(_strip.ColorHSV(13000, 200, 255));  // hue -> RGB
      _strip.setPixelColor(0, color);
      _strip.show();  //  Update strip to match
      delay(50);      //  Pause for a moment
      _strip.clear();
      _strip.show();  //  Update strip to match
      delay(50);      //  Pause for a moment
    }
    _strip.setBrightness(_brightness);
  }

  void colorEditStartAnimation() {
    _strip.setBrightness(150);
    for (int i = 0; i < 3; i++) {                               // For each pixel in strip...
      uint32_t color = _strip.gamma32(_strip.ColorHSV(50000));  // hue -> RGB
      _strip.setPixelColor(0, color);
      _strip.show();  //  Update strip to match
      delay(50);      //  Pause for a moment
      _strip.clear();
      _strip.show();  //  Update strip to match
      delay(50);      //  Pause for a moment
    }
    _strip.setBrightness(_brightness);
  }

  void channelEditStartAnimamtion() {
    _strip.setBrightness(150);
    for (int i = 0; i < 3; i++) {                              // For each pixel in strip...
      uint32_t color = _strip.gamma32(_strip.ColorHSV(4000));  // hue -> RGB
      _strip.setPixelColor(0, color);
      _strip.show();  //  Update strip to match
      delay(50);      //  Pause for a moment
      _strip.clear();
      _strip.show();  //  Update strip to match
      delay(50);      //  Pause for a moment
    }
    _strip.setBrightness(_brightness);
  }

  void channelEditAnimation() {
    uint16_t hue = GLOBAL_MIDI_CHANNEL * (65536 / 16);
    uint32_t color = _strip.gamma32(_strip.ColorHSV(hue));
    _strip.setPixelColor(0, color);
    _strip.show();
  }

  // No animation, displays selected HSV color ONLY
  void inanimate() {
    uint32_t color = _strip.gamma32(_strip.ColorHSV(_hue, _sat, _val));  // hue -> RGB
    _strip.setPixelColor(0, color);
    _strip.show();
  }

  // void pulsar() {}

  // A smooth fade animation inspired by the super knob animation on the Yamaha Montage 8
  void montage() {
    // A smooth fade form HSV(_hue, _sat, _val) to HSV(_hue, _sat, 0) and back
    // This pattern is in four stages, increment, hold, decrement, hold.
    uint16_t timeon = map(animationSpeed, 0, 64, 100, 1000);  // in milliseconds
    uint8_t frames = 60;                                      // frames per half-cycle
    static uint32_t RGB_timer = 0;
    uint16_t threshold = timeon / frames;
    static float stepCount = 0;  // float must be used here instead of int
                                 // This is because of the division (stepCount/frames) later in the code
                                 // Using an integer other wise would return 0 rather a float
                                 // In turn, 0 * pixelVal will ALWAYS return 0, as such the RGB LED will remain off
    static uint8_t stage = 1;    // Begin animation at stage 1

    switch (stage) {
      case 1:
        if (stepCount < frames) {
          if (millis() - RGB_timer < threshold) return;
          stepCount++;
          uint8_t pixelVal = _val * (stepCount / frames);
          uint32_t color = _strip.gamma32(_strip.ColorHSV(_hue, _sat, pixelVal));  // hue -> RGB
          _strip.setPixelColor(0, color);
          _strip.show();
          RGB_timer = millis();
        } else if (stepCount == frames) {
          stepCount = 0;
          uint32_t color = _strip.gamma32(_strip.ColorHSV(_hue, _sat, _val));  // hue -> RGB
          _strip.setPixelColor(0, color);
          _strip.show();
          RGB_timer = millis();  // Reset the RGB_timer variable
          stage = 2;
        }
        break;

      case 2:
        if (millis() - RGB_timer >= threshold) {
          RGB_timer = millis();  // delay(timeOff)
          stage = 3;
        }
        break;

      case 3:

        if (stepCount < frames) {
          if (millis() - RGB_timer < threshold) return;
          stepCount++;
          uint8_t pixelVal = _val * ((frames - stepCount) / frames);
          uint32_t color = _strip.gamma32(_strip.ColorHSV(_hue, _sat, pixelVal));  // hue -> RGB
          _strip.setPixelColor(0, color);
          _strip.show();
          RGB_timer = millis();
        } else if (stepCount == frames) {
          stepCount = 0;
          uint32_t color = _strip.gamma32(_strip.ColorHSV(_hue, _sat, 0));  // hue -> RGB
          _strip.setPixelColor(0, color);
          _strip.show();
          RGB_timer = millis();  // Reset the RGB_timer variable
          stage = 4;
        }
        break;

      case 4:
        if (millis() - RGB_timer >= threshold) {
          RGB_timer = millis();  // delay(timeOff)
          stage = 1;
        }
        break;
    }
  }

  // Modified montage() animation but cycles through 16 colors
  void rainbowMontage() {
    // Hue change will go through 16 cycles beginning with red
    static uint16_t pixelHue = 0;                             // Start from red
    uint32_t cycles = 16;                                     // Number of expected cycles to go through
    uint16_t timeon = map(animationSpeed, 0, 64, 100, 1000);  // in milliseconds
    uint8_t frames = 60;                                      // frames per half-cycle
    static uint32_t RGB_timer = 0;
    uint16_t threshold = timeon / frames;
    static uint8_t stepCount = 0;  // float must be used here instead of int
    static uint8_t stage = 1;      // Begin animation at stage 1

    switch (stage) {
      case 1:
        if (stepCount < frames) {
          if (millis() - RGB_timer < threshold) return;
          stepCount++;
          uint8_t pixelVal = (_val * stepCount) / frames;
          uint32_t color = _strip.gamma32(_strip.ColorHSV(pixelHue, _sat, pixelVal));  // hue -> RGB
          _strip.setPixelColor(0, color);
          _strip.show();
          RGB_timer = millis();
        } else if (stepCount == frames) {
          stepCount = 0;
          uint32_t color = _strip.gamma32(_strip.ColorHSV(pixelHue, _sat, _val));  // hue -> RGB
          _strip.setPixelColor(0, color);
          _strip.show();
          RGB_timer = millis();  // Reset the RGB_timer variable
          stage = 2;
        }
        break;

      case 2:
        if (millis() - RGB_timer >= threshold) {
          RGB_timer = millis();  // delay(timeOff)
          stage = 3;
        }
        break;

      case 3:

        if (stepCount < frames) {
          if (millis() - RGB_timer < threshold) return;
          stepCount++;
          uint8_t pixelVal = (_val * (frames - stepCount)) / frames;
          uint32_t color = _strip.gamma32(_strip.ColorHSV(pixelHue, _sat, pixelVal));  // hue -> RGB
          _strip.setPixelColor(0, color);
          _strip.show();
          RGB_timer = millis();
        } else if (stepCount == frames) {
          stepCount = 0;
          uint32_t color = _strip.gamma32(_strip.ColorHSV(pixelHue, _sat, 0));  // hue -> RGB
          _strip.setPixelColor(0, color);
          _strip.show();
          RGB_timer = millis();  // Reset the RGB_timer variable
          stage = 4;
        }
        break;

      case 4:
        if (millis() - RGB_timer >= threshold) {
          RGB_timer = millis();  // delay(timeOff)
          pixelHue += 65563 / cycles;
          stage = 1;
        }
        break;
    }
  }

  // Rainbow cycle along whole_strip. Pass delay time (in ms) between frames.
  // Modified from Adafruit Neopixel's library example "strandtest"
  // Now runs without code blocking, without a for loop and the delay() function.
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
  // Modified from Adafruit Neopixel's library example "strandtest", theaterChaseRainbow()
  // Now runs without code blocking, without a for loop and the delay() function.
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
      // case RGB_PULSAR:
      //   pulsar();
      //   break;
      case RGB_MONTAGE:
        montage();
        break;
      case RGB_RAINBOWMONTAGE:
        rainbowMontage();
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