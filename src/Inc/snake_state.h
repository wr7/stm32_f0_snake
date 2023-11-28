#pragma once

#include "util.h"
#include "prelude.h"
#include "keyboard.h"
#include "psuedorandom.h"

typedef struct {
  PRNG random_number_generator;
  u8 screen[64 * 4];

  PositionRingBuffer snake_position;
  Direction snake_direction;
  Position fruit_position;

  Position snake_position_buffer[32 * 64];
} SnakeState;

void snake_state_initialize(SnakeState *self);

// Updates the game state and screen buffer
void snake_state_tick(SnakeState *self, Button input);
