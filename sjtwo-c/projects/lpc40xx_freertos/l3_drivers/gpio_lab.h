/**********************************************************************
 * GPIO driver
 * Cmpe146
 * Yucheng Lin(Sean)
 **********************************************************************/
#pragma once

#include <stdbool.h>
#include <stdint.h>
// NOTE: The IOCON is not part of this driver

// 5 ports avaliable for NXP LPC40xx

// GPIO port

typedef enum { GPIO__PORT0 = 0, GPIO__PORT1, GPIO__PORT2, GPIO__PORT3, GPIO__PORT4, GPIO__PORT5 } gpio0__port_e;

// B0:B3 to set function
typedef enum {
  GPIO__FUNCTION0 = 0,
  GPIO__FUNCTION1,
  GPIO__FUNCTION2,
  GPIO__FUNCTION3,
  GPIO__FUNCTION4,
  GPIO__FUNCTION5,
  GPIO__FUNCTION6,
  GPIO__FUNCTION7
} gpio0__function_e;

// GPIO type
typedef struct {
  gpio0__port_e port_number;
  uint8_t pin_number;
} gpio0_s;

gpio0_s gpio0__construct(gpio0__port_e gpio0_port_number, uint8_t gpio0_pin_number);

gpio0_s gpio0__construct_with_function(gpio0__port_e gpio0_port_number, uint8_t gpio0_pin_number,
                                       gpio0__function_e function);

/// Set function
void gpio0__set_function(gpio0_s gpio0, gpio0__function_e function);

/// Should alter the hardware registers to set the pin as input
void gpio0__set_as_input(gpio0_s gpio0);

/// Should alter the hardware registers to set the pin as output
void gpio0__set_as_output(gpio0_s gpio0);

/// Most of the pins are NOT open drain on power-up; this enables pin's open-collector mode
void gpio0__enable_open_drain(gpio0_s gpio0);

/// Should alter the hardware registers to set the pin as high
void gpio0__set_high(gpio0_s gpio0);

/// Should alter the hardware registers to set the pin as low
void gpio0__set_low(gpio0_s gpio0);

/**
 * Should alter the hardware registers to set the pin as low
 *
 * @param {bool} high - true => set pin high, false => set pin low
 */
void gpio0__set(gpio0_s gpio0, bool high);

void gpio0__reset(gpio0_s gpio0, bool high);

/**
 * Should return the state of the pin (input or output, doesn't matter)
 *
 * @return {bool} level of pin high => true, low => false
 */
bool gpio0__get_level(gpio0_s gpio0);

void gpio0__toggle(gpio0_s gpio0);