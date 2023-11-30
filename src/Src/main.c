#include "main.h"
#include "gpio.h"
#include "i2c.h"
#include "screen.h"
#include "constants.h"
#include "keyboard.h"

#include "snake_state.h"
#include "stm32f070xb.h"

int main(void)
{
  initialize_led_pin();

  if(screen_initialize())
    // Enable LED pin
    GPIOA->BSRR |= 1 << LED_PIN;

  SnakeState state; // 4380
  snake_state_initialize(&state);

  while(true)
  {
    Button button = get_button();
    
    snake_state_tick(&state, button);

    // Add delay (to slow down the game)
    for(u32 i = 0; i < 50000; i++) {
      __ASM volatile ("nop");
    }

    screen_update(state.screen);
  }
}

void SystemInit(void)
{
}
