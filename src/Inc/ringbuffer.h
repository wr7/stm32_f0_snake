#pragma once

#include <stdbool.h>
#include <stddef.h>

#define GENERATE_RING_BUFFER_HEADER(ElementType, RingBufferName, FunctionPrefix) \
/* A ring buffer (an O(1) FIFO or LIFO queue/stack)*/ \
typedef struct { \
  /* A pointer to the first element in the buffer. */ \
  ElementType *head; \
  /* The number of elements in the buffer. */ \
  size_t length; \
  /* The start of the memory region available to the ring buffer */ \
  ElementType *start; \
  /* A pointer to the first byte unavailable to the ring buffer */ \
  /* Note: `(size_t) end - (size_t) start` must be a multiple of `sizeof(ElementType)` */ \
  ElementType *end; \
} RingBufferName; \
/*Gets a pointer to the the element at `index`. Note: it is undefined behaviour to call this when `index > length`.*/\
ElementType *FunctionPrefix##_get(RingBufferName self, size_t index); \
/*Gets a pointer to the last element in `self`. Returns `NULL` if the buffer is empty.*/\
ElementType *FunctionPrefix##_last(RingBufferName self); \
/*Tries to push an element to `self`. Returns false on failiure.*/\
bool FunctionPrefix##_push(RingBufferName *self, ElementType element); \
/*Pops an element from the front of the ring buffer (FIFO).*/\
ElementType FunctionPrefix##_pop_front(RingBufferName *self); \
/*Creates an empty ring buffer with a given capacity*/\
RingBufferName FunctionPrefix##_new(ElementType *start, size_t capacity); 


#define GENERATE_RING_BUFFER_SOURCE(ElementType, RingBufferName, FunctionPrefix)\
/*Gets a pointer to the the element at `index`. Note: it is undefined behaviour to call this when `index > length`.*/\
ElementType *FunctionPrefix##_get(RingBufferName self, size_t index) {\
  ElementType *element = self.head + index;\
  if(element >= self.end)\
    element = element - self.end + self.start;\
  \
  return element;\
}\
\
/*Gets a pointer to the last element in `self`. Returns `NULL` if the buffer is empty.*/\
ElementType *FunctionPrefix##_last(RingBufferName self) {\
  if(self.length == 0)\
    return NULL;\
  \
  return FunctionPrefix##_get(self, self.length - 1);\
}\
\
/*Tries to push an element to `self`. Returns false on failiure.*/\
bool FunctionPrefix##_push(RingBufferName *self, ElementType element) {\
  size_t new_length = self->length + 1;\
  \
  if(new_length > (size_t) (self->end - self->start))\
    return false;\
  \
  self->length = new_length;\
  *FunctionPrefix##_last(*self) = element;\
  \
  return true;\
}\
\
/*Pops an element from the front of the ring buffer (FIFO).*/\
ElementType FunctionPrefix##_pop_front(RingBufferName *self) {\
  ElementType *popped_element = FunctionPrefix##_get(*self, 0);\
  \
  self->head = FunctionPrefix##_get(*self, 1);\
  self->length -= 1;\
  \
  return *popped_element;\
}\
\
/*Creates an empty ring buffer with a given capacity*/\
RingBufferName FunctionPrefix##_new(ElementType *start, size_t capacity) {\
  RingBufferName ring_buffer;\
  \
  ring_buffer.start = start;\
  ring_buffer.end = start + capacity;\
  \
  ring_buffer.head = start;\
  ring_buffer.length = 0;\
  \
  return ring_buffer;\
}
