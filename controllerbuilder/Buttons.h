#ifndef BUTTONS_H
#define BUTTONS_H


// start scan
// once read high, begin timer,
// when scan released, calculate time spent on press
// if timer is higher than specific time value, fire abstract interface onLongPress
// else fire onPress

#include <Arduino.h>
#include "ControllerModes.h"
#include "RGB.h"

class Button {
protected:
  uint8_t _pin;
  bool _state = false;
  bool _pState = false;
  unsigned long _lastUpdated = 0;

  unsigned long _scanStartTime;
  uint16_t _longPressTriggerTime = 700;       // hold time required to trigger longpress in ms
  uint16_t _veryLongPressTriggerTime = 1500;  // hold time required to trigger veryLongpress in ms
  uint16_t _resetTriggerTime = 8000;          // hold time required to trigger reset in ms

  static const uint8_t DEBOUNCE_MS = 70;

  bool readHardware();

public:
  // Constructors
  Button(uint8_t pin);
  virtual ~Button() {}

  // Methods
  // Scans hardware, debounces, calls onPress/onRelease on change
  void read();

  void onPress();
  void onLongPress();
  void onVeryLongPress();
  void onResetDevice();
};


#endif