#include "Buttons.h"

Button::Button(uint8_t pin);
  : _pin(pin) {
    pinMode(_pin, INPUT_PULLUP);
}

bool Button::readHardware() {
  static bool reading = false;
    reading = !digitalWrite(_pin);

  return reading;
}

void Button::read() {
  if (millis() - _lastUpdated < DEBOUNCE_MS) return;

  _state = readHardware();

  if (_state == _pState) return;  // no change

  if (_state) {
    // once button pressed
    _scanStartTime = millis();

  } else {
    // once button released
    if (millis() - _scanStartTime >= _veryLongPressTriggerTime) {
      onVeryLongPress();
    } else if (millis() - _scanStartTime >= _longPressTriggerTime) {
      onLongPress();
    } else {
      onPress();
    }
  }

  _lastUpdated = millis();
  _pState = _state;
}

void Button::onPress() {
  if (MODE == MODE_RUNNING) {
  modlock();
  } else {
    // Save and exit edit Mode
    // savePreferencesToEEPROM();
    MODE
  }
}

void Button::onLongPress() {
  MODE  = 
}
void Button::onVeryLongPress() {}

