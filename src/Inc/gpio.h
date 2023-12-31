#pragma once

#include "stm32f070xb.h"

typedef enum {
  GPIO_PORT_A = 0,
  GPIO_PORT_B = 1,
} GpioPort;

typedef enum {
  ALTERNATE_FUNCTION_0,
  ALTERNATE_FUNCTION_1,
  ALTERNATE_FUNCTION_2,
  ALTERNATE_FUNCTION_3,
  ALTERNATE_FUNCTION_4,
  ALTERNATE_FUNCTION_5,
  ALTERNATE_FUNCTION_6,
  ALTERNATE_FUNCTION_7,
} AlternateFunction;

typedef enum {
  RESISTOR_TYPE_NONE = 0,
  RESISTOR_TYPE_PULLUP = 1,
  RESISTOR_TYPE_PULLDOWN = 2,
} ResistorType;

typedef enum {
  PIN_TYPE_PUSH_PULL = 0,
  PIN_TYPE_OPEN_DRAIN = 1,
} PinType;

typedef enum PinSpeed {
  PIN_SPEED_LOW = 0,
  PIN_SPEED_MEDIUM = 1,
  PIN_SPEED_HIGH = 3,
} PinSpeed;

typedef enum PinMode {
  PIN_MODE_INPUT = 0,
  PIN_MODE_OUTPUT = 1,
  PIN_MODE_ALTERNATE_FUNCION = 2,
  PIN_MODE_ANALOG = 3,
} PinMode;

void enable_gpio_port(GpioPort port);
void initialize_led_pin();
