#include "snake_state.h"
#include "prelude.h"
#include "util.h"
#include "screen.h"

volatile Position FOOEY;

// Updates the game state and screen buffer
void snake_state_tick(SnakeState *self, Button input) {
  if(input != BUTTON_NONE) {
    self->snake_direction = (Direction) input;
  }

  Position old_tail = position_ring_buffer_pop_front(&self->snake_position);
  screen_reset_pixel(self->screen, old_tail);

  Position old_head = *position_ring_buffer_get(self->snake_position, self->snake_position.length - 1);

  Position new_head = position_adjacent_to(old_head, self->snake_direction);

  if(new_head.x == 255) {
    snake_state_initialize(self); // Reset the game
    return;
  }

  position_ring_buffer_push(&self->snake_position, new_head);
  screen_set_pixel(self->screen, new_head);
}

void snake_state_initialize(SnakeState *self) {
  self->snake_position = position_ring_buffer_new(self->snake_position_buffer, ARRAY_LENGTH(self->snake_position_buffer));
  self->snake_direction = DIRECTION_RIGHT;

  position_ring_buffer_push(&self->snake_position, (Position){5, 5});
  position_ring_buffer_push(&self->snake_position, (Position){5, 6});
  position_ring_buffer_push(&self->snake_position, (Position){5, 7});
  position_ring_buffer_push(&self->snake_position, (Position){5, 8});

  for(int i = 0; i < ARRAY_LENGTH(self->screen); i++) {
    self->screen[i] = 0;
  }

  // Draw initial segments
  for(int i = 0; i < self->snake_position.length; i++) {
    Position snake_segment = *position_ring_buffer_get(self->snake_position, i);

    screen_set_pixel(self->screen, snake_segment);
  }
}
