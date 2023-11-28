#pragma once
#include "prelude.h"

typedef struct {
  u32 previous;
} PRNG;

PRNG prng_with_default_seed();
u8 prng_next_byte(PRNG *self);
