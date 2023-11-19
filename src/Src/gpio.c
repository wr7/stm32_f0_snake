#include "gpio.h"
#include "constants.h"

void enable_gpio_port(GpioPort port) {
  RCC->AHBENR |= 1 << (17 + port);
}

void enable_led_pin() {
  enable_gpio_port(GPIO_PORT_A);

  GPIOA->PUPDR |= RESISTOR_TYPE_NONE << (LED_PIN * 2);
  GPIOA->OSPEEDR |= PIN_SPEED_LOW << (LED_PIN * 2);
  GPIOA->MODER |= PIN_MODE_OUTPUT << (LED_PIN * 2);
  GPIOA->OTYPER |= PIN_TYPE_PUSH_PULL << LED_PIN;
}
