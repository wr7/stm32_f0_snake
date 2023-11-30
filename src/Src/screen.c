#include "screen.h"
#include "constants.h"
#include "i2c.h"
#include "util.h"

#include <stdbool.h>

void screen_set_pixel(u32 screen_buffer[64], Position position) {
  screen_buffer[position.x] |= 1 << position.y;
}

bool screen_get_pixel(u32 screen_buffer[64], Position position) {
  return (screen_buffer[position.x] & (1 << position.y)) != 0;
}

void screen_reset_pixel(u32 screen_buffer[64], Position position) {
  screen_buffer[position.x] &= ~(1 << position.y);
}

void screen_clear(u32 screen_buffer[64]) {
  // We cannot use the ARRAY_LENGTH macro here because of pointer decay //
  for(usize i = 0; i < 64; i++) {
    screen_buffer[i] = 0;
  }
}

bool screen_initialize() {
  i2c2_initialize();

  // Wait for screen to turn on
  for(u32 i = 0; i < 500000; i++) {
    __ASM volatile ("nop");
  }

  bool ok = true;

  const u8 INIT_PACKET[] = {
    0x00, // command stream mode
    0xa8, 0x3f, 0xd3, 0x00, // set display offset
    0x40, // set start line
    0xa0, // set segment remap
    0xc0, // set com output scan direction
    0xda, 0x12, // set com pin hardware configuration
    0x81, 0x7f, // set contrast
    0xa4, // display contents from ram
    // 0xa5, // entire display on (for debugging)
    0xd5, 0x80, // set oscillator frequency
    0x8d, 0x14, // enable charge pump
    0xaf, // turn on display
  };

  i2c2_start_frame(SCREEN_ADDRESS);

  for(u32 i = 0; i < ARRAY_LENGTH(INIT_PACKET); i++) {
    ok = ok && i2c2_transmit_byte(INIT_PACKET[i]);
  }

  i2c2_end_frame();

  return ok;
}

// Upscales a nibble to a byte. Eg 0101 -> 00110011
const static u8 NIBBLE_UPSCALE_LUT[] = {
  0x00, 0x03, 0x0c, 0x0f, 
  0x30, 0x33, 0x3c, 0x3f,
  0xc0, 0xc3, 0xcc, 0xcf,
  0xf0, 0xf3, 0xfc, 0xff,
};

// Draws a screen buffer to the screen. Note: the screen is 128x64.
// This function automatically upscales a 64x32 image to match the size of the screen.
// 
// The screen buffer should be layed out such that a pixel's value can be read with
// `screen_buffer[column] & (1 << row) != 0`. Where white is true and `black` is `false`.
void screen_update(u32 screen_buffer[64]) {
  for(u8 page = 0; page < 8; page++) {
    const u8 COMMAND_FRAME[] = {
      0x00, // Command stream mode
      0xb0 | page, // Go to page
      0x00, 0x10, // Go to column 0
    };

    i2c2_start_frame(SCREEN_ADDRESS);
    for(fast_u8 i = 0; i < ARRAY_LENGTH(COMMAND_FRAME); i++) {
      i2c2_transmit_byte(COMMAND_FRAME[i]);
    }
    i2c2_end_frame();

    const u8 DATA_FRAME_HEADER[] = {
      0x40, // Data stream mode
      0x00, // The screen for some reason eats
      0x00, // the first two data bytes
    };

    i2c2_start_frame(SCREEN_ADDRESS);
    for(fast_u8 i = 0; i < ARRAY_LENGTH(DATA_FRAME_HEADER); i++) {
      i2c2_transmit_byte(DATA_FRAME_HEADER[i]);
    }

    for(u8 column = 0; column < 128; column++) {
      u8 page_nibble = (screen_buffer[column / 2] >> (page * 4)) & 0xf ;
      i2c2_transmit_byte(NIBBLE_UPSCALE_LUT[page_nibble]);
    }
    i2c2_end_frame();
  }
}
