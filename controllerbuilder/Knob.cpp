#include "Knob.h"
#include "MIDIHelper.h"

#define MIDI_UNSPECIFIED_CHANNEL 0

// ================================== REGULAR KNOB CLASS ================================================
// ======================================================================================================
// Constructors
Knob::Knob(uint8_t potPin, uint8_t CCNumber, uint8_t minCCValue, uint8_t maxCCValue, uint8_t channel, bool isEnabled)
  : _potPin(potPin), _CCNumber(CCNumber), _minCCValue(minCCValue), _maxCCValue(maxCCValue), _channel(channel), _isEnabled(isEnabled) {
  setPinMode();
}

Knob::Knob(uint8_t potPin, uint8_t CCNumber, uint8_t minCCValue, uint8_t maxCCValue, uint8_t channel)
  : Knob(potPin, CCNumber, minCCValue, maxCCValue, channel, true) {
}

Knob::Knob(uint8_t potPin, uint8_t CCNumber, uint8_t minCCValue, uint8_t maxCCValue)
  : Knob(potPin, CCNumber, minCCValue, maxCCValue, GLOBAL_MIDI_CHANNEL) {
}

Knob::Knob(uint8_t potPin, uint8_t CCNumber)
  : Knob(potPin, CCNumber, (uint8_t)0, (uint8_t)127) {
}

Knob::Knob(uint8_t potPin)
  : Knob(potPin, 255) {
}


// Getters
uint8_t Knob::getCCNumber() const {
  return _CCNumber;
}

Knob::MinMax Knob::getMinMax() const {
  return { _minCCValue, _maxCCValue };
}

uint8_t Knob::getMIDIChannel() const {
  if (_channel == MIDI_UNSPECIFIED_CHANNEL) {
    return GLOBAL_MIDI_CHANNEL;
  } else {
    return _channel;
  }
}

// Setters
void Knob::setPinMode() {
  // virtual method
  pinMode(_potPin, INPUT);
}

void Knob::setMIDIChannel(uint8_t channel) {
  _channel = channel;
}

void Knob::setAnalogMin(uint16_t minAnalogValue) {
  _minAnalogValue = constrain(minAnalogValue, 0, 1023);  // 12 bits read, declared in setup()
}

void Knob::setAnalogMax(uint16_t maxAnalogValue) {
  _maxAnalogValue = constrain(maxAnalogValue, 0, 1023);  // 12 bits read, declared in setup()
}

void Knob::setCCMin(uint8_t minCCValue) {
  _minCCValue = constrain(minCCValue, 0, 127);
}

void Knob::setCCMax(uint8_t maxCCValue) {
  _maxCCValue = constrain(maxCCValue, 0, 127);
}

// Methods
void Knob::enable() {
  _isEnabled = true;
}

void Knob::disable() {
  _isEnabled = false;
}

void Knob::readKnob() {
  _potState = analogRead(_potPin);
}

void Knob::validateAnalogRead() {
  _potState = (_potState >= _minAnalogValue && _potState <= _maxAnalogValue) ? _potState : constrain(_potState, _minAnalogValue, _maxAnalogValue);
  // The ternary operator allows for conditional error logging OUT_OF_RANGE_ERROR should need be in future
  // else, the statement "_potState = constrain(_potState, _minAnalogValue, _maxAnalogValue)" would suffice
}

void Knob::update() {
  if (Knob::_isEnabled) {
    readKnob();
    validateAnalogRead();
    _midiState = map(_potState, _minAnalogValue, _maxAnalogValue, _minCCValue, _maxCCValue);
    _potIncrement = abs(_midiState - _midiPState);

    if (_potIncrement > _potThreshold) {
      snapshot = millis();
    }

    _potTimer = millis() - snapshot;

    if (_potTimer < POT_TIMEOUT) {
      if (_midiState != _midiPState) {
        controlChange(getMIDIChannel(), _CCNumber, _midiState);
        _midiPState = _midiState;
      }
      _potPState = _potState;
    }
  }
}
// ======================================================================================================
// ======================================================================================================
