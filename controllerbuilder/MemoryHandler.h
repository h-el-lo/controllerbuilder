#pragma once
#include "RGB.h"
#include "MIDIHelper.h"

#include "EEPROMWearLevel.h"

#define LAYOUT_VERSION 0
#define AMOUNT_OF_INDEXES 2
#define USER_SETTINGS_EXISTS 0
#define USER_SETTINGS 1

struct Settings {
  uint8_t MIDI_CHANNEL;
  RGB_Mode rgb_mode;
  uint16_t hue;
  uint8_t sat;
  uint8_t val;
  uint8_t brightness;
};

extern Settings defaultSettings;
extern Settings userSettings;
extern bool userSettingsExists;


inline void saveUserSettings() {
  userSettings = {
    GLOBAL_MIDI_CHANNEL,
    MY_PIXEL.getMode(),
    MY_PIXEL.getHue(),
    MY_PIXEL.getSat(),
    MY_PIXEL.getVal(),
    MY_PIXEL.getBrightness(),
  };

  EEPROMwl.put(USER_SETTINGS, userSettings);
  Serial.println("User Settings Saved via saveUserSettings().");
}

inline void retrieveUserSettings() {
  EEPROMwl.get(USER_SETTINGS, userSettings);
  GLOBAL_MIDI_CHANNEL = userSettings.MIDI_CHANNEL;
  MY_PIXEL.setMode(userSettings.rgb_mode);
  MY_PIXEL.setColor(userSettings.hue, userSettings.sat, userSettings.val, userSettings.brightness);
}

inline void settingsReset() {
  userSettings = defaultSettings;  // reset userSettings from defaultSettings
  GLOBAL_MIDI_CHANNEL = userSettings.MIDI_CHANNEL;
  MY_PIXEL.setMode(userSettings.rgb_mode);
  MY_PIXEL.setColor(userSettings.hue, userSettings.sat, userSettings.val, userSettings.brightness);

  // Save reset user settings to EEPROM
  EEPROMwl.put(USER_SETTINGS, userSettings);
  Serial.println("User Settings Reset via settingsReset().");
}