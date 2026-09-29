#include "RGB.h"

enum Mode {
MODE_RUNNING,
MODE_CHANNEL_EDIT,
MODE_COLOR_EDIT,
};

extern Mode MODE;


inline void setColor() {
MY_PIXEL.setColor();
}



