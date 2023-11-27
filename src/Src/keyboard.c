#include "gpio.h"
#include "keyboard.h"
#include "prelude.h"
#include "stm32f070xb.h"

void initialize_keyboard() {
  enable_gpio_port(GPIO_PORT_A);

  GPIOA->PUPDR |= 
    RESISTOR_TYPE_PULLDOWN << GPIO_PUPDR_PUPDR0_Pos | 
    RESISTOR_TYPE_PULLDOWN << GPIO_PUPDR_PUPDR1_Pos | 
    RESISTOR_TYPE_PULLDOWN << GPIO_PUPDR_PUPDR3_Pos | 
    RESISTOR_TYPE_PULLDOWN << GPIO_PUPDR_PUPDR4_Pos;
}

Button get_button() {
  u16 pin_state = GPIOA->IDR;

  if(pin_state & 1)
    return BUTTON_UP;
  else if((pin_state & (1 << 1)) != 0)
    return BUTTON_DOWN;
  else if((pin_state & (1 << 3)) != 0)
    return BUTTON_LEFT;
  else if((pin_state & (1 << 4)) != 0)
    return BUTTON_RIGHT;
  else
    return BUTTON_NONE;
}
