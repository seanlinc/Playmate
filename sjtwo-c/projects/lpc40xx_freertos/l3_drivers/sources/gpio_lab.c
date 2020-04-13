/**********************************************************************
 * GPIO driver
 * Cmpe146
 * Yucheng Lin(Sean)
 **********************************************************************/
#include "gpio_lab.h"
#include "lpc40xx.h"
#include <stdbool.h>
#include <stdint.h>

// NOTE: The IOCON is not part of this driver

// Private function
static volatile uint32_t *gpio0__get_iocon(gpio0_s gpio0) {

  volatile uint32_t *port_iocon_base =
      (volatile uint32_t *)LPC_IOCON_BASE + ((uint32_t)gpio0.port_number * UINT32_C(32));

  volatile uint32_t *pin_iocon = (port_iocon_base + gpio0.pin_number);

  return pin_iocon;
};

static const LPC_GPIO_TypeDef *gpio0_map[6] = {LPC_GPIO0, LPC_GPIO1, LPC_GPIO2, LPC_GPIO3, LPC_GPIO4, LPC_GPIO5};

static LPC_GPIO_TypeDef *gpio0__get_map(gpio0_s gpio0) { return (LPC_GPIO_TypeDef *)gpio0_map[gpio0.port_number]; };

/*******************************************************************************
 *
 *                      P U B L I C    F U N C T I O N S
 *
 ******************************************************************************/

gpio0_s gpio0__construct(gpio0__port_e gpio0_port_number, uint8_t gpio0_pin_number) {
  gpio0_s gpio0;
  gpio0.port_number = gpio0_port_number;
  gpio0.pin_number = gpio0_pin_number;
  return gpio0;
};

gpio0_s gpio0__construct_with_function(gpio0__port_e gpio0_port_number, uint8_t gpio0_pin_number,
                                       gpio0__function_e function) {
  gpio0_s gpio0;
  gpio0.port_number = gpio0_port_number;
  gpio0.pin_number = gpio0_pin_number;
  volatile uint32_t *pin_iocon = gpio0__get_iocon(gpio0);
  *pin_iocon &= ~(UINT32_C(7) << 0);
  *pin_iocon |= ((uint32_t)function << 0);
  return gpio0;
};

/// Set function
void gpio0__set_function(gpio0_s gpio0, gpio0__function_e function) {
  volatile uint32_t *pin_iocon = gpio0__get_iocon(gpio0);
  *pin_iocon &= ~(UINT32_C(7) << 0);
  *pin_iocon |= ((uint32_t)function << 0);
};

/// Should alter the hardware registers to set the pin as input
void gpio0__set_as_input(gpio0_s gpio0) { gpio0__get_map(gpio0)->DIR &= ~(1 << gpio0.pin_number); };

/// Should alter the hardware registers to set the pin as output
void gpio0__set_as_output(gpio0_s gpio0) { gpio0__get_map(gpio0)->DIR |= (1 << gpio0.pin_number); };

/// Most of the pins are NOT open drain on power-up; this enables pin's open-collector mode
void gpio0__enable_open_drain(gpio0_s gpio0) {
  volatile uint32_t *pin_iocon = gpio0__get_iocon(gpio0);
  *pin_iocon |= (UINT32_C(1) << 10);
};

/// Should alter the hardware registers to set the pin as high
void gpio0__set_high(gpio0_s gpio0) { gpio0__get_map(gpio0)->PIN |= (1 << gpio0.pin_number); };

/// Should alter the hardware registers to set the pin as low
void gpio0__set_low(gpio0_s gpio0) { gpio0__get_map(gpio0)->PIN &= ~(1 << gpio0.pin_number); };

/**
 * Should alter the hardware registers to set the pin as low
 *
 * @param {bool} high - true => set pin high, false => set pin low
 */
void gpio0__set(gpio0_s gpio0, bool high) {

  const uint32_t pin_io_mask = (1 << gpio0.pin_number);

  if (high) {
    gpio0__get_map(gpio0)->SET |= pin_io_mask;
  } else {
    gpio0__get_map(gpio0)->CLR |= pin_io_mask;
  }
};

/**
 * Should return the state of the pin (input or output, doesn't matter)
 *
 * @return {bool} level of pin high => true, low => false
 */
bool gpio0__get_level(gpio0_s gpio0) { return (gpio0__get_map(gpio0)->PIN & (UINT32_C(1) << gpio0.pin_number)); };

void gpio0__toggle(gpio0_s gpio0) {
  if (gpio0__get_level(gpio0)) {
    gpio0__set_low(gpio0);
  } else {
    gpio0__set_high(gpio0);
  }
}