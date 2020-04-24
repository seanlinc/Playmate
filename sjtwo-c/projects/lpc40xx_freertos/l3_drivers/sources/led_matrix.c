/*********************************************************
 * This is a driver for VS1053 Codec board with amplifier
 * Designed for SJSU SJ2 dev board
 *
 * Written by Yucheng Lin(Sean), Bo An, and Alex Chavez
 *
 *********************************************************/

#include "led_matrix.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "delay.h"
#include "gpio.h"
#include "time.h"

void led__init() {
  pin = gpio__construct_as_output(2, 0);
  gpio__reset(pin);
}

void led__show() {
  uint8_t *ptr, *end, p, bitMask;
  ptr = pixels;
  end = ptr + number_of_bytes;
  p = *ptr++;
  bitMask = 0x80;

  for (;;) {
    if (p & bitMask) {
      // data ONE high
      // min: 550 typ: 700 max: 5,500
      gpio__set(pin);
      delay__ns(550);
      // min: 450 typ: 600 max: 5,000
      gpio__reset(pin);
      delay__ns(450);
    } else {
      // data ZERO high
      // min: 200  typ: 350 max: 500
      gpio__set(pin);
      delay__ns(200);
      // data low
      // min: 450 typ: 600 max: 5,000
      gpio__reset(pin);
      delay__ns(450);
    }
    if (bitMask >>= 1) {
      // Move on to the next pixel
      asm("nop;");
    } else {
      if (ptr >= end)
        break;
      p = *ptr++;
      bitMask = 0x80;
    }
  }
}
