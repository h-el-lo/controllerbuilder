#include "MIDIUSB.h"
#include <ResponsiveAnalogRead.h>

const int N_ANALOGS = {8}
int analogPins = { A0, A1, A2, A3, A7, A8, A9, A10 };
const int N_POTS = 2;

int potPin[N_POTS] = { A0, A1, A2, A3, A10 };
int potCC[N_POTS] = { 24, 25, 26, 27, 28 };

int potReading[N_POTS] = { 0 };
int potState[N_POTS] = { 0 };
int potPState[N_POTS] = { 0 };

int midiState[N_POTS] = { 0 };
int midiPState[N_POTS] = { 0 };

byte potThreshold = 2;
const int POT_TIMEOUT = 300;
unsigned long pPotTime[N_POTS] = { 0 };
unsigned long potTimer[N_POTS] = { 0 };

byte POT_CH = 0;

float snapMultiplier = 0.01;
ResponsiveAnalogRead responsivePot[N_ANALOGS] = {};

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

        controlChange(POT_CH, potCC[i], midiState[i]);
        MidiUSB.flush();
        
        midiPState[i] = midiState[i];
      }
      potPState[i] = potState[i];
    }
  }
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = { 0x0B, 0xB0 | channel, control, value };
  MidiUSB.sendMIDI(event);
}