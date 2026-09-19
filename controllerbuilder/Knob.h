#ifndef KNOB_H
#define KNOB_H

#include <Arduino.h>
#include "MIDIHelper.h"

// Analog Inputs for Knobs and Faders alike

// ========================  KNOB  =========================
// =========================================================
// Regular knobs connected directly to MCU's ADC pins

class Knob {
protected:
  const uint8_t _potPin;  // Connected to MCU, ADS or MUX
  uint8_t _CCNumber;
  uint16_t _minAnalogValue = 0;     // Maximum analog value from potentiometer readings (0 - 1023) Read in 10 bits
  uint16_t _maxAnalogValue = 1023;  // Maximum analog value from potentiometer readings (0 - 1023) Read in 10 bits
  uint8_t _minCCValue = 0;          // Minimum CC value
  uint8_t _maxCCValue = 127;        // Maximum CC value
  int16_t _potState, _potPState;
  uint8_t _midiState = 0;
  uint8_t _midiPState = 0;
  uint8_t _channel = 0;
  bool _isEnabled;

  unsigned long snapshot = millis();  // Pot time recorder snapshot
  uint16_t _potIncrement = 0;
  uint16_t _potTimer = 0;
  static constexpr uint8_t _potThreshold = 3;
  static constexpr uint16_t POT_TIMEOUT = 300;

  // Constructors
public:
  Knob(uint8_t potPin, uint8_t CCNumber, uint8_t min, uint8_t max, uint8_t channel, bool isEnabled);
  Knob(uint8_t potPin, uint8_t CCNumber, uint8_t min, uint8_t max, uint8_t channel);
  Knob(uint8_t potPin, uint8_t CCNumber, uint8_t min, uint8_t max);
  Knob(uint8_t potPin, uint8_t CCNumber);
  Knob(uint8_t potPin);

  struct MinMax {
    uint8_t min, max;
  };  // A struct to return the min and max values of the knob when getMinMax() is called.

  // Getters
  uint8_t getCCNumber() const;
  uint8_t getMin() const;
  uint8_t getMax() const;
  MinMax getMinMax() const;  // Returns min and max CC values of knob
  virtual uint8_t getMIDIChannel() const;

  // Setters
  virtual void setPinMode();
  virtual void setMIDIChannel(uint8_t channel);
  void setAnalogMin(uint16_t minAnalogValue);
  void setAnalogMax(uint16_t maxAnalogValue);
  void setCCMin(uint8_t CCMinValue);
  void setCCMax(uint8_t CCMaxValue);

  // Methods
  void enable();
  void disable();
  virtual void readKnob();
  void validateAnalogRead();
  virtual void update();
};
// =========================================================
// =========================================================

#endif