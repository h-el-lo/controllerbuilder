#include "Joystick.h"
#include "MIDIHelper.h"

// Constructors
Joystick::Joystick(uint8_t xAxisPin, uint8_t yAxisPin, uint8_t yUpperCC, uint8_t yLowerCC)
  : ResponsiveAnalogRead(0, true, snapMultiplier), _yAxisPin(yAxisPin), _yUpperCC(yUpperCC), _yLowerCC(yLowerCC), _yCenter(512) {
  pinMode(_yAxisPin, INPUT);
  ResponsiveAnalogRead().setAnalogResolution(1023);
}

Joystick::Joystick(uint8_t xAxisPin, uint8_t yAxisPin)
  : Joystick(xAxisPin, yAxisPin, 1, 2) {  // Modulation Wheel CC01 [upper], Breath Controller CC02 [lower]
}

// Getters

// Setters
void Joystick::setDeadzoneRange() {
  // Pass
}

// Methods
void Joystick::readYAxis() {
  _yState = analogRead(_yAxisPin);
}

void Joystick::updateXAxis() {
  PitchWheel.update();
}

void Joystick::updateYAxis() {
  readYAxis();
  _delta = abs(_yState - _yPrevState);

  if (_delta > _threshold) {
    _yLastUpdatedTime = millis();
  }

  timePassed = millis() - _yLastUpdatedTime;
  _yState = constrain(_yState, 0, 1023);

  if (timePassed < TIMEOUT) {

    // if within deadone range
    if ((_yState <= _yCenter + deadzoneRange) && (_yState >= _yCenter - deadzoneRange)) {
      // If Wheel reading of Y Axis is within deadzone range, set value of both yUpperCC and yLowerCC to 0
      if (!wheel_is_centered) {
        controlChange(GLOBAL_MIDI_CHANNEL, _yLowerCC, 0);
        controlChange(GLOBAL_MIDI_CHANNEL, _yUpperCC, 0);
        wheel_is_centered = true;
      }

      // if outside of dead range zone
    } else if ((_yState >= _yCenter + deadzoneRange) || (_yState <= _yCenter - deadzoneRange)) {

      uint8_t CCToSend;
      if (_yState >= _yCenter + deadzoneRange) {
        // Upper section of Y Axis
        midiState = map(_yState, _yCenter, _yMax, 0, 127);
        CCToSend = _yUpperCC;
      } else if (_yState <= _yCenter - deadzoneRange) {
        // Lower section of Y Axis
        midiState = map(_yState, _yCenter, _yMin, 0, 127);
        CCToSend = _yLowerCC;
      }

      if (midiState != lastMidiState) {
        controlChange(GLOBAL_MIDI_CHANNEL, CCToSend, midiState);
        lastMidiState = midiState;
        wheel_is_centered = false;
      }
      // if reading is out of potentiometer's physical range
    } else {
      // Log PitchWheel Error
      Serial.println("PITCH_WHEEL_ERROR: Y_POT reading out of recognizable range.");
    }
    _yPrevState = _yState;
  }
}

void Joystick::update() {
  updateXAxis();
  updateYAxis();
}
