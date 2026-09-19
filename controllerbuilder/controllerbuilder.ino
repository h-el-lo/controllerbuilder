#include "MIDIHelper.h"
#include "Joystick.h"
#include "Knob.h"
#include "DamperPedal.h"
#include "ExpressionPedal.h"

// =================================  GLOBAL VARIABLES =================================
uint8_t GLOBAL_MIDI_CHANNEL = 0;  // MIDI Channel 1
Damper_Pedal DamperPedal(5);
Expression_Pedal ExpressionPedal(6);
Pitch_Wheel PitchWheel;
Joystick joystick(A9, A8);
// =====================================================================================

// ======================================  KNOBS  ======================================
const uint8_t NUM_OF_KNOBS = 5;
Knob knobset[NUM_OF_KNOBS]{
  Knob(A6, 7),
  Knob(A0, 24),
  Knob(A1, 25),
  Knob(A2, 26),
  Knob(A3, 27),
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
  // ExpressionPedal.update();
  //===========================================================================

  //===========================  READ ALL KNOBS  ==============================
  for (uint8_t i = 0; i < NUM_OF_KNOBS; i++) {
    knobset[i].update();
  }
  //===========================================================================
}

// Add support for WS2812B RGB LED
