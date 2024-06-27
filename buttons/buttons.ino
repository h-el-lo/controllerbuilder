#include "MIDIUSB.h"

const int N_BUTTONS = 3;

int buttonPin[N_BUTTONS] = { 4, 3, 2 };

int buttonState[N_BUTTONS] = { 0 };
int buttonPState[N_BUTTONS] = { 0 };  // previous state of the button

int lastDebounceTime[N_BUTTONS] = { 0 };
int debounceTimer[N_BUTTONS] = { 0 };


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  for (int i = 0; i < N_BUTTONS; i++) {
    pinMode(buttonPin[i], INPUT_PULLUP);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < N_BUTTONS; i++) {

    buttonState[i] = digitalRead(buttonPin[i]);

    debounceTimer[i] = millis() - lastDebounceTime;

    if (buttonState[i] != buttonPState) {

      lastDebounceTime[i] = millis();

      if (buttonState[i] == LOW) {
        Serial.print("Button ");
        Serial.print(i);
        Serial.print(" ");
        Serial.println("on");
      } else {
        Serial.print("Button ");
        Serial.print(i);
        Serial.print(" ");
        Serial.println("off");
      }
    }
  }
}


void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = { 0x0B, 0xB0 | channel, control, value };
  MidiUSB.sendMIDI(event);
}
