#include "MIDIHelper.h"
#include "Joystick.h"
#include "Knob.h"
#include "DamperPedal.h"

// =================================  GLOBAL VARIABLES =================================
uint8_t GLOBAL_MIDI_CHANNEL = 0;  // MIDI Channel 1
Damper_Pedal DamperPedal(5);
Joystick joystick(A9, A8);
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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(921600);
}

void loop() {
  DamperPedal.update();  // Read and update sustain pedal
  joystick.update();     // Read and update joystick

  //========================  READ EXPRESSION PEDAL  ==========================
  // EXPRESSIONPEDAL.update();
  //===========================================================================

  //===========================  READ ALL KNOBS  ==============================
  for (uint8_t i = 0; i < NUM_OF_KNOBS; i++) {
    knobset[i].update();
  }
  //===========================================================================
}

// Add support for exression pedal, four pin 6.35 mm jack, pin four for presence checker, pin A6
// Add support for WS2812B RGB LED
