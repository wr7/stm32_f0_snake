#pragma once
#include <stdbool.h>

#include "prelude.h"

bool initialize_screen();

// Draws a screen buffer to the screen. Note: the screen is 128x64.
// This function automatically upscales a 64x32 image to match the size of the screen.
// 
// The screen buffer should be layed out such that a pixel's value can be read with
// `screen_buffer[column * 4 + row/8] & (row % 8) != 0`. Where white is true and `black` is `false`.
void update_screen(u8 screen_buffer[64 * 4]);
