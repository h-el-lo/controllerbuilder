#include "MIDIUSB.h"
#include <ResponsiveAnalogRead.h>

// Global Channel
int channel = 0;

// Global Analog Input Variables
const int N_ANALOGS = 8;
int analogPins[N_ANALOGS] = { A0, A1, A2, A3, A7, A8, A9, A10 };
float snapMultiplier = 0.01;
ResponsiveAnalogRead responsivePot[N_ANALOGS] = {};

// Potentiometer Variables
const int N_POTS = 5;
int potPin[N_POTS] = { A3, A2, A1, A0, A10 };
int potCC[N_POTS] = { 24, 25, 26, 27, 7 };

int potReading[N_POTS] = { 0 };
int potState[N_POTS] = { 0 };
int potPState[N_POTS] = { 0 };

int midiState[N_POTS] = { 0 };
int midiPState[N_POTS] = { 0 };

byte potThreshold = 15;
const int POT_TIMEOUT = 300;
unsigned long pPotTime[N_POTS] = { 0 };
unsigned long potTimer[N_POTS] = { 0 };

// Modulation Wheel Variables
int modWheel = A8;  // JoyY
int modReading = 0;
int modMidiState = 0;
int modMidiPState = 0;
int modState = 0;
int modPrevState = 0;
int modTimer = 0;
int pModTime = 0;

// Pitch Wheel Variables
int pitchWheel = A9;  // JoyX
int pitchReading = 0;
int pitchMidiState = 0;
int pitchMidiPState = 0;
int pitchState = 0;
int pitchPrevState = 0;
byte pitchThreshold = 3;

// Sustain Pedal Variables
int sustainPin = 7;
int susState = 0;
int susPrevState = 0;



void setup() {
  // put your setup code here, to run once:
  pinMode(sustainPin, INPUT_PULLUP);
  Serial.begin(9600);

  for (int i = 0; i < N_ANALOGS; i++) {
    responsivePot[i] = ResponsiveAnalogRead(0, true, snapMultiplier);
    responsivePot[i].setAnalogResolution(1023);
  }
}



void loop() {
  // Reading of all potentiometers other than Pitch and Modulation Wheels
  //=========================================================
  for (int i = 0; i < N_POTS; i++) {

    potReading[i] = analogRead(potPin[i]);
    responsivePot[i].update(potReading[i]);
    potState[i] = responsivePot[i].getValue();
    midiState[i] = map(potState[i], 0, 1023, 0, 127);

    int potVar = abs(potState[i] - potPState[i]);

    if (potVar > potThreshold) {
      pPotTime[i] = millis();
    }

    potTimer[i] = millis() - pPotTime[i];

    if (potTimer[i] < POT_TIMEOUT) {
      if (midiState[i] != midiPState[i]) {
        controlChange(channel, potCC[i], midiState[i]);
        MidiUSB.flush();
        // delay(5);
        midiPState[i] = midiState[i];
      }
      potPState[i] = potState[i];
    }
  }
  //=========================================================


  // Modulation Wheel (Joystick Y)
  // =========================================================
  // The Modulation Wheel has to be written separately,
  // because of the difference in the range of values
  // - 127 to 127, precisely.

  int modReading = analogRead(modWheel);
  // Index of the pitchWheel pin in the analog pins array.
  int modIndex = findIndex(analogPins, N_ANALOGS, modWheel);
  responsivePot[modIndex].update(modReading);
  modState = responsivePot[modIndex].getValue();
  modMidiState = map(modReading, 0, 1023, -127, 127);
  int modVar = abs(modState - modPrevState);

  if (modVar > potThreshold) {
    pModTime = millis();
  }

  modTimer = millis() - pModTime;

  if (modTimer < POT_TIMEOUT) {
    if (modMidiState != modMidiPState) {
      if (modMidiState >= 0) {
        // Send Modulation coarse (CC 1)
        controlChange(channel, 1, modMidiState);
        MidiUSB.flush();
        // delay(5);
      } else {
        // Send modulationm LSB fine/smooth (CC 33)
        controlChange(channel, 33, abs(modMidiState));
        MidiUSB.flush();
      }
      modMidiPState = modMidiState;
    }
    modPrevState = modState;
  }
  //==========================================================


  // Pitch Wheel (Joystick X)
  //=========================================================
  int pitchReading = analogRead(pitchWheel);
  // Index of the pitchWheel pin in the analog pins array.
  int pitchIndex = findIndex(analogPins, N_ANALOGS, pitchWheel);
  responsivePot[pitchIndex].update(pitchReading);
  pitchState = responsivePot[pitchIndex].getValue();
  pitchMidiState = map(pitchReading, 1023, 0, 0, 16383);

  int pitchVar = abs(pitchState - pitchPrevState);

  if (pitchVar > pitchThreshold) {

    if (pitchMidiState != pitchMidiPState) {
      pitchBend(channel, pitchMidiState);
      MidiUSB.flush();
      pitchPrevState = pitchState;
      // delay(5);
    }
    pitchMidiPState = pitchMidiState;
  }
  //==========================================================


  // Sustain Pedal
  //=========================================================
  int susRead = !digitalRead(sustainPin);
  int susState = map(susRead, 0, 1, 0, 127);

  if (susState != susPrevState) {
    controlChange(channel, 64, susState);
    MidiUSB.flush();
    susPrevState = susState;
    delay(5);
  }
  //==========================================================
}



void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = { 0x0B, 0xB0 | channel, control, value };
  MidiUSB.sendMIDI(event);
}

void pitchBend(byte channel, int value) {
  midiEventPacket_t pitchBend = { 0x0E, 0xE0 | channel, value & 0x7F, (value >> 7) & 0x7F };
  MidiUSB.sendMIDI(pitchBend);
}

int findIndex(int arr[], int size, int target) {
  for (int i = 0; i < size; i++) {
    if (arr[i] == target) {
      return i;  // Return the index if a match is found.
    }
  }

  return -1;  // Return =1 if the target is not found in the array.
}