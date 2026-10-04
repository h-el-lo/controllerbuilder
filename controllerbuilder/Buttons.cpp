#include "Buttons.h"
#include "MemoryHandler.h"

Button::Button(uint8_t pin)
  : _pin(pin) {
  pinMode(_pin, INPUT_PULLUP);
}

bool Button::readHardware() {
  static bool reading = false;
  reading = !digitalRead(_pin);
  return reading;
}

void Button::read() {
  if (millis() - _lastUpdated < DEBOUNCE_MS) return;
  _state = readHardware();

  // if ((_state == _pState) && (millis() - _scanStartTime >= _resetTriggerTime)) {
  //   // if button press is SUSTAINED (_state == _pState) for resetTriggerTime in ms
  //   onResetDevice();
  // } else
  if (_state == _pState) {
    return;
  }


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
    // modlock();
  } else if (MODE == MODE_CHANNEL_EDIT) {
    // Save settings to EEPROM and return to MODE_RUNNING;
    saveUserSettings();
    MODE = MODE_RUNNING;
    MY_PIXEL.runningModeStartAnimation();

  } else if (MODE == MODE_COLOR_EDIT) {
    // Save settings to EEPROM and return to MODE_RUNNING;
    saveUserSettings();
    MODE = MODE_RUNNING;
    MY_PIXEL.runningModeStartAnimation();
  }
}

void Button::onLongPress() {
  // Only change to other Channel edit mode if current mode is MODE_RUNNING
  if (MODE == MODE_RUNNING) {
    MODE = MODE_CHANNEL_EDIT;
    MY_PIXEL.channelEditStartAnimamtion();
  }
}

void Button::onVeryLongPress() {
  if (MODE == MODE_RUNNING) {
    MODE = MODE_COLOR_EDIT;
    MY_PIXEL.colorEditStartAnimation();
  }
}

void Button::onResetDevice() {
  settingsReset();
  MODE = MODE_RUNNING;
}
