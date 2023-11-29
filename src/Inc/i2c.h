#pragma once

#include "constants.h"
#include "gpio.h"
#include "stm32f070xb.h"
#include<stdbool.h>

void i2c2_initialize();

/// Starts an I2C frame. `initialize_i2c2` must be called
/// before this function.
///
/// NOTE: i2c_transmit_byte must be called atleast once
/// before `i2c_end_frame`.
void i2c2_start_frame(u8 address);
void i2c2_end_frame();
bool i2c2_transmit_byte(u8 byte);
