#pragma once

#include <Arduino.h>
#include "MIDIHelper.h"

// A Struct is created in order to avoid the possibility of conflicting locals
struct Damper_Pedal {
  uint8_t susPin;
  uint8_t susState = 0;
  uint8_t susPrevState = 0;
  uint8_t damperDebounceTime = 100;  // 100ms
  unsigned long lastUpdatedTime = millis();

  Damper_Pedal(uint8_t suspin)
    : susPin(suspin) {
    pinMode(susPin, INPUT_PULLUP);
  }

  void update() {
    if (millis() - lastUpdatedTime >= damperDebounceTime) {
      susState = map(!digitalRead(susPin), 0, 1, 0, 127);

      if (susState != susPrevState) {
        controlChange(GLOBAL_MIDI_CHANNEL, 64, susState);
        susPrevState = susState;
        lastUpdatedTime = millis();
      }
    }
  }
};

extern Damper_Pedal DamperPedal;  // on pin
