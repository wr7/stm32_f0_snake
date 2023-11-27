#include "main.h"
#include "gpio.h"
#include "i2c.h"
#include "screen.h"
#include "constants.h"
#include "keyboard.h"

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

  u8 screen[64 * 4] = {0};
  
  screen[12]=0xaa;

  while(true)
  {
    Button button = get_button();
    screen[0] = 0;
    screen[4] = 0;
    screen[8] = 0;

    switch (button) {
    case BUTTON_NONE:
      // screen[4] = 2; break;
      break;
    case BUTTON_UP:
      screen[4] = 1; break;
    case BUTTON_DOWN:
      screen[4] = 4; break;
    case BUTTON_LEFT:
      screen[0] = 2; break;
    case BUTTON_RIGHT:
      screen[8] = 2; break;
    }

    update_screen(screen);
  }
}

void SystemInit(void)
{
}
