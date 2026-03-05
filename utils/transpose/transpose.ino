#include "MIDIUSB.h"

const int transposeCC = 20;
int transposeValue = 64;

void setup() {
  // put your setup code here, to run once:
}

void loop() {

  transposeValue++;
  if (transposeValue > 127) {
    transposeValue =0;
  }
  sendTransposeCC(transposeValue);
  delay(1000);

}

void sendTransposeCC(byte value) {
  midiEventPacket_t event = { 0x0B, 0xB0 | 0, transposeCC, value };
  MidiUSB.sendMIDI(event);
}