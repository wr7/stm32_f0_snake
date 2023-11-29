#include "util.h"
#include "ringbuffer.h"

GENERATE_RING_BUFFER_SOURCE(Position, PositionRingBuffer, position_ring_buffer)

bool direction_is_opposite_of(Direction self, Direction other) {
  switch(self) {
    case DIRECTION_UP: return other == DIRECTION_DOWN;
    case DIRECTION_DOWN: return other == DIRECTION_UP;
    case DIRECTION_LEFT: return other == DIRECTION_RIGHT;
    case DIRECTION_RIGHT: return other == DIRECTION_LEFT;
  }

  __builtin_unreachable();
}

// Gets a position adjacent to `origin` in the direction `direction`
// or returns (255, 255) if the position is offscreen.
Position position_adjacent_to(Position origin, Direction direction) {
  Position adjacent_position = origin;

  switch(direction) {
    case DIRECTION_UP: adjacent_position.y -= 1; break;
    case DIRECTION_DOWN: adjacent_position.y += 1; break;
    case DIRECTION_LEFT: adjacent_position.x -= 1; break;
    case DIRECTION_RIGHT: adjacent_position.x += 1; break;
  }

  if(adjacent_position.x >= 64 || adjacent_position.y >= 32) { // Set x and y to sentinel values
    adjacent_position.x = 255;
    adjacent_position.y = 255;
  }

  return adjacent_position;
}
