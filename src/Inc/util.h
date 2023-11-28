#pragma once

#include "prelude.h"
#include "ringbuffer.h"

typedef enum {
  DIRECTION_UP = 1,
  DIRECTION_DOWN = 2,
  DIRECTION_LEFT = 3,
  DIRECTION_RIGHT = 4,
} Direction;

typedef struct {
  u8 x;
  u8 y;
} Position;

// Gets a position adjacent to `origin` in the direction `direction`
// or returns (255, 255) if the position is offscreen.
Position position_adjacent_to(Position origin, Direction direction);

/* A ring buffer (an O(1) FIFO or LIFO queue/stack)*/
GENERATE_RING_BUFFER_HEADER(Position, PositionRingBuffer, position_ring_buffer)

// The following is just the macro above but expanded (for documentation with IntelliSense and other LSPs)

/*Gets a pointer to the the element at `index`. Note: it is undefined behaviour to call this when `index > length`.*/
Position *position_ring_buffer_get(PositionRingBuffer self, size_t index);
/*Gets a pointer to the last element in `self`. Returns `NULL` if the buffer is empty.*/
Position *position_ring_buffer_last(PositionRingBuffer self);
/*Tries to push an element to `self`. Returns false on failiure.*/
_Bool position_ring_buffer_push(PositionRingBuffer *self, Position element);
/*Pops an element from the front of the ring buffer (FIFO).*/
Position position_ring_buffer_pop_front(PositionRingBuffer *self);
/*Creates an empty ring buffer with a given capacity*/
PositionRingBuffer position_ring_buffer_new(Position *start, size_t capacity);

// MACRO END
//
