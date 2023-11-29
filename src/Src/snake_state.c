#include "snake_state.h"
#include "util.h"
#include "screen.h"
#include "psuedorandom.h"

static void respawn_fruit(SnakeState *self) {
  while(true) {
    u8 new_x = prng_next_byte(&self->random_number_generator) % 64;
    u8 new_y = prng_next_byte(&self->random_number_generator) % 32;

    self->fruit_position = (Position){new_x, new_y};

    if(!screen_get_pixel(self->screen, self->fruit_position)) {
      break;
    }
  }

  screen_set_pixel(self->screen, self->fruit_position);
}

// Updates the game state and screen buffer
void snake_state_tick(SnakeState *self, Button input) {
  if(input != BUTTON_NONE) {
    self->snake_direction = (Direction) input;
  }

  Position old_head = *position_ring_buffer_get(self->snake_position, self->snake_position.length - 1);
  Position new_head = position_adjacent_to(old_head, self->snake_direction);

  if(new_head.x == 255) {
    snake_state_initialize(self); // Reset the game
    return;
  }

  if(screen_get_pixel(self->screen, new_head)) {
    if(new_head.x == self->fruit_position.x && new_head.y == self->fruit_position.y) {
      respawn_fruit(self);
    } else {
      // We just hit another snake segment
      snake_state_initialize(self); // Reset the game
      return;
    }
  } else {
    // We did not hit a fruit, so remove the end tail segment
    Position old_tail = position_ring_buffer_pop_front(&self->snake_position);
    screen_reset_pixel(self->screen, old_tail);
  }

  position_ring_buffer_push(&self->snake_position, new_head);
  screen_set_pixel(self->screen, new_head);
}

void snake_state_initialize(SnakeState *self) {
  self->random_number_generator = prng_with_default_seed();
  self->snake_position = position_ring_buffer_new(self->snake_position_buffer, ARRAY_LENGTH(self->snake_position_buffer));
  self->snake_direction = DIRECTION_DOWN;

  // Clear screen //
  for(int i = 0; i < ARRAY_LENGTH(self->screen); i++) {
    self->screen[i] = 0;
  }

  // Create and draw initial snake segments //
  position_ring_buffer_push(&self->snake_position, (Position){5, 5});
  position_ring_buffer_push(&self->snake_position, (Position){5, 6});
  position_ring_buffer_push(&self->snake_position, (Position){5, 7});

  for(int i = 0; i < self->snake_position.length; i++) {
    Position snake_segment = *position_ring_buffer_get(self->snake_position, i);

    screen_set_pixel(self->screen, snake_segment);
  }

  // Add border around screen to make the edges more visible //

  for(u8 i = 0; i < 64; i++) {
    screen_set_pixel(self->screen, (Position){i, 0});
    screen_set_pixel(self->screen, (Position){i, 31});
  }
  for(u8 i = 1; i < 32; i++) {
    screen_set_pixel(self->screen, (Position){0, i});
    screen_set_pixel(self->screen, (Position){63, i});
  }

  respawn_fruit(self);
}
