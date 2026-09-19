#pragma once

#include "Knob.h"
#include "MIDIHelper.h"

struct Pitch_Wheel : public Knob {

private:
  uint8_t _potThreshold = 1;
  static constexpr uint16_t POT_TIMEOUT = 400;  // May change later
  uint8_t DEADZONE_RANGE = 2;
  uint16_t _minAnalogValue = 4;
  uint16_t _maxAnalogValue = 1014;
  uint16_t wheelCenter = 487;
  bool wheel_is_centered = false;
  uint16_t midiValue = 8192;
  uint16_t lastMidiValue = 0;

  void centerWheel() {
    centerPitchWheel();
  }


public:
  // Constructors
  Pitch_Wheel()
    : Knob(A9) {
    // No-op
  }

  void update() override {
    if (_isEnabled) {
      readKnob();
      validateAnalogRead();

      _potIncrement = abs(_potState - _potPState);

      if (_potIncrement > _potThreshold) {
        snapshot = millis();
      }

      _potTimer = millis() - snapshot;

      if (_potTimer < POT_TIMEOUT) {

        if ((_potState >= wheelCenter - DEADZONE_RANGE) && (_potState <= wheelCenter + DEADZONE_RANGE)) {
          if (!wheel_is_centered) {
            centerWheel();
            wheel_is_centered = true;
          }

        } else {

          if (_potState >= wheelCenter + DEADZONE_RANGE) {
            // Lower/left section of Y Axis
            midiValue = map(constrain(_potState, _minAnalogValue, _maxAnalogValue), wheelCenter, _maxAnalogValue, 8192, 0);

          } else if (_potState <= wheelCenter - DEADZONE_RANGE) {
            // Upper/right section of Axis
            midiValue = map(constrain(_potState, _minAnalogValue, _maxAnalogValue), _minAnalogValue, wheelCenter, 16383, 8192);
          }

          if (midiValue != lastMidiValue) {
            pitchBend(GLOBAL_MIDI_CHANNEL, midiValue);
            lastMidiValue = midiValue;
            wheel_is_centered = false;
          }
          _potPState = _potState;
        }
      }
    }
  }
};