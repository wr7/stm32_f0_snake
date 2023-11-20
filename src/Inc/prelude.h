#pragma once

#include <stdint.h>
#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

typedef unsigned int u32;
typedef signed int i32;
typedef unsigned short u16;
typedef signed short i16;
typedef unsigned char u8;
typedef signed char i8;

typedef uint_fast32_t fast_u32;
typedef int_fast32_t fast_i32;
typedef uint_fast16_t fast_u16;
typedef int_fast16_t fast_i16;
typedef uint_fast8_t fast_u8;
typedef int_fast8_t fast_i8;
