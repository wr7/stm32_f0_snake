#include "psuedorandom.h"

PRNG prng_with_default_seed() {
  return (PRNG){1483364093};
}

u8 prng_next_byte(PRNG *self) {
  u32 new_random = (self->previous + (self->previous >> 31)) ^ (self->previous << 3);

  self->previous = new_random;

  return new_random & 0xff;
}
