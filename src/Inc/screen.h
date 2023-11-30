#pragma once
#include <stdbool.h>

#include "util.h"

void screen_set_pixel(u32 screen_buffer[64], Position position);
bool screen_get_pixel(u32 screen_buffer[64], Position position);
void screen_reset_pixel(u32 screen_buffer[64], Position position);

void screen_clear(u32 screen_buffer[64]);

bool screen_initialize();

// Draws a screen buffer to the screen. Note: the screen is 128x64.
// This function automatically upscales a 64x32 image to match the size of the screen.
// 
// The screen buffer should be layed out such that a pixel's value can be read with
// `screen_buffer[column] & (1 << row) != 0`. Where white is true and `black` is `false`.
void screen_update(u32 screen_buffer[64]);
