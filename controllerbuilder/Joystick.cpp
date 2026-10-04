#include "Joystick.h"
#include "MIDIHelper.h"
#include "ControllerModes.h"

// Constructors
Joystick::Joystick(uint8_t xAxisPin, uint8_t yAxisPin, uint8_t yUpperCC, uint8_t yLowerCC)
  : ResponsiveAnalogRead(0, true, snapMultiplier), _yAxisPin(yAxisPin), _yUpperCC(yUpperCC), _yLowerCC(yLowerCC), _yCenter(512), _joyButton(7) {
  pinMode(_yAxisPin, INPUT);
  ResponsiveAnalogRead().setAnalogResolution(1023);
}

Joystick::Joystick(uint8_t xAxisPin, uint8_t yAxisPin)
  : Joystick(xAxisPin, yAxisPin, 1, 2) {  // Modulation Wheel CC01 [upper], Breath Controller CC02 [lower]
}

// Methods
void Joystick::readYAxis() {
  _yState = analogRead(_yAxisPin);
}

void Joystick::channelAndColorUpdate() {
  uint8_t potPin = A9;
  uint16_t minAnalog = 5;
  uint16_t maxAnalog = 1013;

  uint8_t minPoint = 1;
  uint8_t maxPoint = 5;
  uint8_t midPoint = (minPoint + maxPoint) / 2;

  uint8_t reading = 0;
  static uint8_t lastReading = 255;  // Sentinel value
  static bool buttonIsPressed = false;

  reading = map(analogRead(potPin), maxAnalog, minAnalog, minPoint, maxPoint);
  if (reading != lastReading) {
    if (!buttonIsPressed) {
      if (reading == minPoint) {
        if (MODE == MODE_CHANNEL_EDIT) {
          GLOBAL_MIDI_CHANNEL--;
          GLOBAL_MIDI_CHANNEL = ((GLOBAL_MIDI_CHANNEL % 16) + 16) % 16;

        } else if (MODE == MODE_COLOR_EDIT) {
          MY_PIXEL.previousColorMode();
        }
        buttonIsPressed = true;

      } else if (reading == maxPoint) {
        if (MODE == MODE_CHANNEL_EDIT) {
          GLOBAL_MIDI_CHANNEL++;
          GLOBAL_MIDI_CHANNEL = ((GLOBAL_MIDI_CHANNEL % 16) + 16) % 16;

        } else if (MODE == MODE_COLOR_EDIT) {
          MY_PIXEL.nextColorMode();
        }
        buttonIsPressed = true;
      }

    } else {
      if (reading == midPoint)
        buttonIsPressed = false;
    }
    lastReading = reading;
  }
}

void Joystick::updateXAxis() {
  if (MODE == MODE_RUNNING) {
    PitchWheel.update();
  } else {
    Joystick::channelAndColorUpdate();
  }
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
    } 
    _yPrevState = _yState;
  }
}

void Joystick::updateJoyButton() {
  _joyButton.read();
}

void Joystick::update() {
  updateXAxis();
  updateYAxis();
  updateJoyButton();
}
