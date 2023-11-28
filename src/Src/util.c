#include "util.h"
#include "ringbuffer.h"

GENERATE_RING_BUFFER_SOURCE(Position, PositionRingBuffer, position_ring_buffer)

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
