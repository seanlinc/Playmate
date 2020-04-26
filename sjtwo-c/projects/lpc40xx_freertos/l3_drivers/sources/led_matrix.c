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
#include <string.h>

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

  while (led__busy)
    ;

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

/*!
  @brief   Set a pixel's color using separate red, green and blue
           components. If using RGBW pixels, white will be set to 0.
  @param   n  Pixel index, starting from 0.
  @param   r  Red brightness, 0 = minimum (off), 255 = maximum.
  @param   g  Green brightness, 0 = minimum (off), 255 = maximum.
  @param   b  Blue brightness, 0 = minimum (off), 255 = maximum.
*/

void led__set_rgb_color(uint16_t n, uint8_t red, uint8_t green, uint8_t blue) {
  if (n < number_of_led) {
    if (brightness) {
      red = (red * brightness) >> 8;
      green = (green * brightness) >> 8;
      blue = (blue * brightness) >> 8;
    }

    uint8_t *ptr;
    if (white_offset == red_offset) {
      ptr = &pixels[n * 3];
    } else {
      ptr = &pixels[n * 4];
      ptr[white_offset] = 0;
    }
    ptr[red_offset] = red;
    ptr[green_offset] = green;
    ptr[blue_offset] = blue;
  }
}

void led__set_color(uint16_t n, uint32_t color) {
  if (n < number_of_led) {
    uint8_t *ptr;
  }
}

void led__set_brightness(uint8_t brightness) {
  uint8_t new_brightness = brightness + 1;
  if (new_brightness != brightness) {
    uint8_t c, *ptr, old_brightness = brightness - 1;
    uint16_t scale;

    if (old_brightness == 0) {
      scale = 0;
    } else if (brightness == 255) {
      scale = 65535 / old_brightness;
    } else {
      scale = (((uint16_t)new_brightness << 8) - 1) / old_brightness;
    }

    for (uint16_t i = 0; i < number_of_bytes; i++) {
      c = *ptr;
      *ptr++ = (c * scale) >> 8;
    }
    brightness = new_brightness;
  }
}

void clear() { memset(pixels, 0, number_of_bytes); }