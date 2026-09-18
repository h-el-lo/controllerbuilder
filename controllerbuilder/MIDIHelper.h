#pragma once

#include <Arduino.h>
#include <MIDIUSB.h>

extern uint8_t GLOBAL_MIDI_CHANNEL;

// These functions are defined as inline to prevent multiple definition errors if 
// the header file passes through multiple translation units TLUs

inline void controlChange(uint8_t channel, uint8_t control, uint8_t value) {
  midiEventPacket_t event = { 0x0B, 0xB0 | channel, control, value };
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

inline void pitchBend(uint8_t channel, int value) {
  midiEventPacket_t event = { 0x0E, 0xE0 | channel, value & 0x7F, (value >> 7) & 0x7F };
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

inline void centerPitchWheel() {
  uint8_t value = 0;
  midiEventPacket_t event = { 0x0E, 0xE0 | GLOBAL_MIDI_CHANNEL, value & 0x7F, (value >> 7) & 0x7F };
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}
