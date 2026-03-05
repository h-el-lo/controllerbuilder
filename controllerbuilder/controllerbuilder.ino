#include "MIDIHelper.h"
#include "Joystick.h"
#include "Knob.h"
#include "TouchSensor.h"

// =================================  GLOBAL VARIABLES =================================
uint8_t GLOBAL_MIDI_CHANNEL = 0;  // MIDI Channe 1
// Global Analog Input Variables
const int N_ANALOGS = 8;
int analogPins[N_ANALOGS] = { A0, A1, A2, A3, A7, A8, A9, A10 };
// =====================================================================================

// ==================================  SUSTAIN PEDAL  ==================================
uint8_t sustainPin = 7;
uint8_t susRead;
uint8_t susState = 0;
uint16_t susPrevState = 0;
// =====================================================================================

// ====================================  JOYSTICK  =====================================
Joystick joystick(2, 3);
// =====================================================================================

// ======================================  KNOBS  ======================================
const uint8_t NUM_OF_KNOBS = 5;
Knob knobset[NUM_OF_KNOBS]{
  Knob(A3, 24),
  Knob(A2, 25),
  Knob(A1, 26),
  Knob(A0, 27),
  Knob(A10, 7),
};
// =====================================================================================

// ==================================  TOUCH SENSORS  ==================================
const uint8_t NUM_OF_TOUCH_SENSORS = 4;
TouchSensor touchSensors[NUM_OF_TOUCH_SENSORS] = {
  TouchSensor(2, 24),
  TouchSensor(4, 25),
  TouchSensor(5, 26),
  TouchSensor(6, 27),
};
// =====================================================================================





void setup() {
  // put your setup code here, to run once:
  pinMode(sustainPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  //=========================  READ SUSTAIN PEDAL  ============================
  susRead = !digitalRead(sustainPin);
  susState = map(susRead, 0, 1, 0, 127);

  if (susState != susPrevState) {
    controlChange(GLOBAL_MIDI_CHANNEL, 64, susState);
    susPrevState = susState;
  }
  //===========================================================================

  //========================  READ EXPRESSION PEDAL  ==========================
  // EXPRESSIONPEDAL.update();
  //===========================================================================

  //============================  READ JOYSTICK  ==============================
  joystick.update();
  //===========================================================================

  //===========================  READ ALL KNOBS  ==============================
  // Reading of all potentiometers other than Pitch and Modulation Wheels
  for (uint8_t i = 0; i < NUM_OF_KNOBS; i++) {
    knobset[i].update();
  }
  //===========================================================================

  //=======================  READ ALL TOUCH SENSORS  ==========================
  for (uint8_t i = 0; i < NUM_OF_TOUCH_SENSORS; i++) {
    touchSensors[i].update();
  }
  //===========================================================================

}


// Add support for exression pedal, four pin 6.35 mm jack, pin four for presence checker
// Add support for WS2812B RGB LED

