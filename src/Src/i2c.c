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
  // (as described in the STM32F030x4/6/8/C reference manual) //

  const u32 PRESC = 0x1; // (nibble)
  const u32 SCLDEL = 0x2; // (nibble)
  const u32 SDADEL = 0x2; // (nibble)
  const u32 SCLH = 0x0F; // (byte)
  const u32 SCLL = 0x0F; // (byte)

  I2C2->TIMINGR = (PRESC << 28) | (SCLDEL << 20) | (SDADEL << 16) | (SCLH << 8) | SCLL;

  // Enable I2C2 //
  I2C2->CR1 = I2C_CR1_PE | I2C_CR1_ANFOFF;
}

/// Starts an I2C frame. `initialize_i2c2` must be called
/// before this function.
///
/// NOTE: i2c_transmit_byte must be called atleast once
/// before `i2c_end_frame`.
void i2c_start_frame(u8 address) {
  I2C2->ICR = I2C_ICR_NACKCF;

  I2C2->CR2 = 
    ((u32) address) << 1U |
    I2C_CR2_RELOAD | 
    1U << I2C_CR2_NBYTES_Pos |
    I2C_CR2_START;
}

bool i2c_transmit_byte(u8 byte) {
  while(true) {
    u32 isr = I2C2->ISR;

    if((isr & I2C_ISR_NACKF_Msk) != 0) {
      I2C2->ICR = I2C_ICR_NACKCF;
      return false;
    } else if((isr & I2C_ISR_TXIS_Msk) != 0) {
      I2C2->TXDR = byte;
      break;
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
  for(u32 i = 0; i < 20; i++) {
    __ASM volatile ("nop");
  }
}
