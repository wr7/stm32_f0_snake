#include "main.h"
#include "gpio.h"
#include "constants.h"

#include "stm32f070xb.h"

int main(void)
{
  initialize_led_pin();

  GPIOA->BSRR |= 1 << LED_PIN;

  while (1)
  {
  }
}

void SystemInit(void)
{
}
