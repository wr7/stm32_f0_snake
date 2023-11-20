#include "constants.h"
#include "gpio.h"
#include "i2c.h"
#include "stm32f070xb.h"

#include<stdbool.h>

void initialize_i2c2() {
  const u32 SDA_PIN = 14;
  const u32 SCL_PIN = 13;
  
  enable_gpio_port(GPIO_PORT_B);

  // Configure SDA and SCL pins //
  GPIOB->OTYPER |= 
    PIN_TYPE_OPEN_DRAIN << SDA_PIN |
    PIN_TYPE_OPEN_DRAIN << SCL_PIN ;
  GPIOB->OSPEEDR |=
    PIN_SPEED_LOW << SDA_PIN |
    PIN_SPEED_LOW << SCL_PIN;
  GPIOB->PUPDR |=
    RESISTOR_TYPE_NONE << SDA_PIN |
    RESISTOR_TYPE_NONE << SCL_PIN;
  GPIOB->MODER |=
    PIN_MODE_ALTERNATE_FUNCION << (SDA_PIN * 2) |
    PIN_MODE_ALTERNATE_FUNCION << (SCL_PIN * 2);
  GPIOB->AFR[1] |= 
    ALTERNATE_FUNCTION_5 << (SDA_PIN * 4 % 32) |
    ALTERNATE_FUNCTION_5 << (SCL_PIN * 4 % 32);
  // Enable I2C2 clock //
  RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

  ////// TIMING PARAMETERS //////
  //// I2C Speed: 10 kHz
  //// Speed Mode: Standard
  //// Fall/Rise Time: 0

  const u32 I2C_PARAMETERS = 0x20887F7F;
  I2C2->TIMINGR = I2C_PARAMETERS;

  // Enable I2C2 //
  // TODO: try one CR1 write
  I2C2->CR1 = I2C_CR1_ANFOFF;
  I2C2->CR1 = I2C_CR1_PE | I2C_CR1_ANFOFF;
}

/// Starts an I2C frame. `initialize_i2c2` must be called
/// before this function.
///
/// NOTE: i2c_transmit_byte must be called atleast once
/// before `i2c_end_frame`.
void i2c_start_frame(u8 address) {
  u32 new_cr2 = 
    ((u32) address) << 1U |
    I2C_CR2_RELOAD | 
    1U << I2C_CR2_NBYTES_Pos;

  I2C2->CR2 = new_cr2;

  I2C2->ICR = I2C_ICR_NACKCF;
  // TODO: try one CR2 write
  I2C2->CR2 = new_cr2 | I2C_CR2_START;
}

bool i2c_transmit_byte(u8 byte) {
  while(true) {
    u32 isr = I2C2->ISR;

    if((isr & I2C_ISR_NACKF_Msk) != 0) {
      I2C2->ICR = I2C_ICR_NACKCF;
      return false;
    } else if((isr & I2C_ISR_TXIS_Msk) != 0) {
      I2C2->TXDR = byte;
      // TODO: try resetting TXIS mask. Maybe this fixes the hardware bug????
      break;
      // Another fix `!=` -> `==`
    } else if((isr & I2C_ISR_BUSY_Msk) == 0) {
      return false;
    }
  }

  // Wait for TCR //

  while(true) {
    u32 isr = I2C2->ISR;

    if((isr & I2C_ISR_NACKF_Msk) != 0) {
      I2C2->ICR = I2C_ICR_NACKCF;

      return false;
    } else if((isr & I2C_ISR_TCR_Msk) != 0) {
      I2C2->CR2 |= I2C_CR2_RELOAD | 1 << I2C_CR2_NBYTES_Pos;

      return true;
    }
  }
}

void i2c_end_frame() {
  I2C2->CR2 |= I2C_CR2_STOP;

  // Weird hardware bug ??????

  // If we try to send another I2C frame too soon,
  // SCL will be pulled low until the MCU is reset
  for(u32 i = 0; i < 500; i++) {
    __ASM volatile ("nop");
  }
}
