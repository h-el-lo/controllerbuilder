#include "MIDIUSB.h"
#include <ResponsiveAnalogRead.h>


int channel = 0;

// Global Analog Input Variables
const int N_ANALOGS = 8;
int analogPins[N_ANALOGS] = { A0, A1, A2, A3, A7, A8, A9, A10 };
float snapMultiplier = 0.01;
ResponsiveAnalogRead responsivePot[N_ANALOGS] = {};

// Pitch Wheel Variables
int pitchWheel = A1;  // JoyX
int pitchReading = 0;
int pitchMidiState = 0;
int pitchMidiPState = 0;
int pitchState = 0;
int pitchPrevState = 0;
byte pitchThreshold = 3;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  for (int i = 0; i < N_ANALOGS; i++) {
    responsivePot[i] = ResponsiveAnalogRead(0, true, snapMultiplier);
    responsivePot[i].setAnalogResolution(1023);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // Reading of the Pitch Wheel (Joystick X)
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
      delay(20);
    }
    pitchMidiPState = pitchMidiState;
  }
  //==========================================================
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