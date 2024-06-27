#include <MIDIUSB.h>

int channel = 0;

int sustainPin = 15;
int susState = 0;
int susPrevState = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(sustainPin, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  int susRead = !digitalRead(sustainPin);
  int susState = map(susRead, 0, 1, 0, 127);

  if (susState != susPrevState) {
    controlChange(channel, 64, susState);
    MidiUSB.flush();
    susPrevState = susState;
    delay(20);
  }
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = { 0x0B, 0xB0 | channel, control, value };
  MidiUSB.sendMIDI(event);
}