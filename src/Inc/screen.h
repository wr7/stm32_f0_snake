#pragma once
#include <stdbool.h>

#include "prelude.h"
#include "util.h"

void screen_set_pixel(u8 screen_buffer[64 * 4], Position position);
bool screen_get_pixel(u8 screen_buffer[64 *4], Position position);
void screen_reset_pixel(u8 screen_buffer[64 * 4], Position position);

bool initialize_screen();

// Draws a screen buffer to the screen. Note: the screen is 128x64.
// This function automatically upscales a 64x32 image to match the size of the screen.
// 
// The screen buffer should be layed out such that a pixel's value can be read with
// `screen_buffer[column * 4 + row/8] & (row % 8) != 0`. Where white is true and `black` is `false`.
void update_screen(u8 screen_buffer[64 * 4]);
