#pragma once

#include "util.h"

typedef enum {
  BUTTON_NONE = 0,
  BUTTON_UP = DIRECTION_UP,
  BUTTON_DOWN = DIRECTION_DOWN,
  BUTTON_LEFT = DIRECTION_LEFT,
  BUTTON_RIGHT = DIRECTION_RIGHT,
} Button;

Button get_button();
void initialize_keyboard();
