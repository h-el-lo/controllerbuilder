#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <Arduino.h>
#include "PitchWheel.h"
#include <ResponsiveAnalogRead.h>

class Joystick : public ResponsiveAnalogRead {
private:
  // X variables
  Pitch_Wheel PitchWheel;

  // Y variables
  uint8_t _yAxisPin;
  uint8_t _yUpperCC;
  uint8_t _yLowerCC;

  uint16_t _yMin = 0;
  uint16_t _yCenter = 516;  // Physical center of axis reading
  uint16_t _yMax = 1021;

  uint8_t _threshold = 8;
  uint8_t deadzoneRange = 4;
  uint16_t _delta;

  uint16_t _yState;
  uint16_t _yPrevState;
  uint8_t midiState;
  uint16_t lastMidiState = 0;

  unsigned long _yLastUpdatedTime;
  uint16_t timePassed;
  uint16_t TIMEOUT = 250;
  bool wheel_is_centered = false;
  // ResponsiveAnalogRead
  inline static const float snapMultiplier = 0.01;


public:
  // Constructors
  Joystick(uint8_t xAxisPin, uint8_t yAxisPin, uint8_t yUpperCC, uint8_t yLowerCC);
  Joystick(uint8_t xAxisPin, uint8_t yAxisPin);

  // Getters

  // Setters
  void setDeadzoneRange();

  // Methods
  void readYAxis();
  void updateXAxis();
  void updateYAxis();
  void update();
};


#endif