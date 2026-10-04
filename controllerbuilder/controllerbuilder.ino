#include "MIDIHelper.h"
#include "Joystick.h"
#include "Knob.h"
#include "DamperPedal.h"
#include "ExpressionPedal.h"
#include "ControllerModes.h"
#include "RGB.h"
#include "MemoryHandler.h"

// =================================  GLOBAL VARIABLES =================================
Mode MODE = MODE_RUNNING;
uint8_t GLOBAL_MIDI_CHANNEL = 0;  // MIDI Channel 1

Settings defaultSettings = { 0, RGB_MONTAGE, 10000, 255, 255, 50 };
Settings userSettings;
bool userSettingsExists;

Damper_Pedal DamperPedal(5);
// Expression_Pedal ExpressionPedal(6);
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

  EEPROMwl.begin(LAYOUT_VERSION, AMOUNT_OF_INDEXES);

  EEPROMwl.get(USER_SETTINGS_EXISTS, userSettingsExists);
  if (!userSettingsExists) {
    userSettings = defaultSettings;
    saveUserSettings();
    userSettingsExists = true;
    EEPROMwl.put(USER_SETTINGS_EXISTS, userSettingsExists);

  } else {
    retrieveUserSettings();
  }
}

void loop() {
  switch (MODE) {
    case MODE_RUNNING:
      MY_PIXEL.update();  // update RGB

      DamperPedal.update();  // Read and update sustain pedal
      joystick.update();     // Read and update joystick
      // ExpressionPedal.update(); // Read and update expression pedal

      //===========================  READ ALL KNOBS  ==============================
      for (uint8_t i = 0; i < NUM_OF_KNOBS; i++) {
        knobset[i].update();
      }
      //===========================================================================
      break;
    case MODE_COLOR_EDIT:
      MY_PIXEL.editColorAndAnimation();
      joystick.updateJoyButton();
      joystick.updateXAxis();
      break;
    case MODE_CHANNEL_EDIT:
      MY_PIXEL.channelEditAnimation();
      joystick.updateJoyButton();
      joystick.updateXAxis();
      break;
  }
}