#include "main.h"
#include "gpio.h"
#include "i2c.h"
#include "screen.h"
#include "constants.h"
#include "keyboard.h"

#include "snake_state.h"
#include "stm32f070xb.h"

// size = 156
// typedef struct {
//   I2C_TypeDef i2c2;
//   u32 padding1;
//   RCC_TypeDef rcc;
//   u32 padding2[2];
//   GPIO_TypeDef gpiob;
// } LogStruct;

// volatile LogStruct LOG;

int main(void)
{
  initialize_i2c2();

  for(u32 i = 0; i < 500000; i++) {
    __ASM volatile ("nop");
  }

  initialize_screen();

  initialize_led_pin();

  if(initialize_screen())
    // Enable LED pin
    GPIOA->BSRR |= 1 << LED_PIN;

  SnakeState state;
  snake_state_initialize(&state);

  while(true)
  {
    Button button = get_button();
    
    snake_state_tick(&state, button);

    update_screen(state.screen);
  }
}

void SystemInit(void)
{
}
