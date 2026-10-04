#pragma once

// enum Mode: int;

enum Mode : int {
  MODE_RUNNING,       // Regular running mode
  MODE_CHANNEL_EDIT,  // Channel edit mode
  MODE_COLOR_EDIT,    // Color and animation edit mode
};

extern Mode MODE;
