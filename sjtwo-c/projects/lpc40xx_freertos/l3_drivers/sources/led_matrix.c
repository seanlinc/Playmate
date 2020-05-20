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
#include <stdlib.h>
#include <string.h>

#include "delay.h"
#include "gpio.h"

void led__init() {
  led_pin = gpio__construct(0, 6);
  gpio__set_as_output(led_pin);
  gpio__reset(led_pin);
  led__update_type();
  led__update_length();

  puts("Finish LED Matrix Initilization");
}

void led__show() {

  if (!pixels) {
    puts("Pixels buffer not set");
    return;
  }

  uint8_t *ptr, *end, p, bitMask;
  ptr = pixels;
  end = ptr + number_of_bytes;
  p = *ptr++;
  bitMask = 0x80;

  // while (led__busy)
  //   ;

  // delay__ms(300);

  for (;;) {
    if (p & bitMask) {
      // data ONE high
      // min: 550 typ: 700 max: 5,500
      gpio__set(led_pin);
      // delay__us(0.7);
      __asm("nop; nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop; nop;");
      // min: 450 typ: 600 max: 5,000
      gpio__reset(led_pin);
      // delay__us(0.6);
      __asm("nop; nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop;");
      // led__delay_ns(350);
      // puts("HIGH");
    } else {
      // data ZERO high
      // min: 200  typ: 350 max: 500
      gpio__set(led_pin);
      // delay__us(0.35);
      // led__delay_ns(200);
      // data low
      // min: 450 typ: 600 max: 5,000
      __asm("nop; nop; nop; nop; nop; nop; nop;");

      gpio__reset(led_pin);
      // delay__us(0.8);
      // led__delay_ns(400);
      // puts("LOW");
      __asm("nop; nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop;");
    }
    if (bitMask >>= 1) {
      // Move on to the next pixel
      __asm__("nop;");
    } else {
      if (ptr >= end)
        break;
      p = *ptr++;
      // printf("Pixel value: %d\n", p);
      bitMask = 0x80;
    }
    // printf("Pixel: %p\n", ptr);
    // printf("Pixel value: %d\n", *ptr)
  }
  // puts("LED shows");
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
    if (led_brightness) {
      red = (red * led_brightness) >> 8;
      green = (green * led_brightness) >> 8;
      blue = (blue * led_brightness) >> 8;
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
  uint8_t red, green, blue;

  // printf("Color: %d\n", color);

  if (n < number_of_led) {
    uint8_t *ptr;
    red = (uint8_t)(color >> 16);
    green = (uint8_t)(color >> 8);
    blue = (uint8_t)color;

    if (led_brightness) {
      red = (red * led_brightness) >> 8;
      green = (green * led_brightness) >> 8;
      blue = (blue * led_brightness) >> 8;
    }

    if (white_offset == red_offset) {
      ptr = &pixels[n * 3];
    } else {
      ptr = &pixels[n * 4];
      uint8_t white = (uint8_t)(color >> 24);
      ptr[white_offset] = led_brightness ? ((white * led_brightness) >> 8) : white;
    }
    ptr[red_offset] = red;
    // printf("Ptr: %p\n", ptr);
    // printf("Pixel Red: %d\n", ptr[red_offset]);
    ptr[green_offset] = green;
    // printf("Pixel green: %d\n", ptr[green_offset]);
    ptr[blue_offset] = blue;
    // printf("Pixel blue: %d\n", ptr[blue_offset]);
  }
}

uint32_t led__change_color(uint8_t red, uint8_t green, uint8_t blue) {
  return ((uint32_t)red << 16 | ((uint32_t)green << 8) | blue);
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
    led_brightness = new_brightness;
  }
}

void led__update_type() {
  bool oldThreeBytesPerPixel = (white_offset == red_offset); // false if RGBW
  white_offset = (NEO_GRB >> 6) & 0b11;                      // See notes in header file
  red_offset = (NEO_GRB >> 4) & 0b11;                        // regarding R/G/B/W offsets
  green_offset = (NEO_GRB >> 2) & 0b11;
  blue_offset = NEO_GRB & 0b11;

  if (pixels) {
    bool newThreeBytesPerPixel = (white_offset == red_offset);
    if (newThreeBytesPerPixel != oldThreeBytesPerPixel)
      led__update_length(number_of_led);
  }
}

void led__update_length() {
  free(pixels);
  number_of_bytes = number_of_led * ((white_offset == red_offset) ? 3 : 4);
  if ((pixels = (uint8_t *)malloc(number_of_bytes))) {
    memset(pixels, 0, number_of_bytes);
  } else {
    number_of_bytes = 0;
  }
}

void led__draw_pixel(int16_t x, int16_t y, uint32_t color) {
  if ((x < 0) || (y < 0) || (x > matrix_width) || (y > matrix_height)) {
    return;
  }

  int pixelOffset;
  // LED type NEO GRB
  uint8_t corner = NEO_GRB & NEO_MATRIX_CORNER;
  uint16_t minor, major, majorScale;

  minor = x;
  major = y;

  if (corner & NEO_MATRIX_RIGHT) {
    minor = matrix_width - 1 - minor;
  }

  if (corner & NEO_MATRIX_BOTTOM) {
    major = matrix_height - 1 - major;
  }

  if ((NEO_GRB & NEO_MATRIX_AXIS) == NEO_MATRIX_ROWS) {
    majorScale = matrix_width;
  } else {
    led__swap_uint16_t(major, minor);
    majorScale = matrix_height;
  }

  /* Determine actual major axis of matrix */
  if ((NEO_GRB & NEO_MATRIX_SEQUENCE) == NEO_MATRIX_PROGRESSIVE) {
    // All lines in same order
    pixelOffset = major * majorScale + minor;
  } else {
    if (major & 1) {
      pixelOffset = (major + 1) * majorScale - 1 - minor;
    } else {
      pixelOffset = major * majorScale + minor;
    }
  }

  led__set_color(pixelOffset, pass_through_flag ? pass_through_color : color);
}

void led__set_pass_through_color(uint32_t color) {
  pass_through_color = color;
  pass_through_flag = true;
}
void led__clean_pass_through() { pass_through_flag = false; }

void led__swap_uint16_t(uint16_t a, uint16_t b) {
  uint16_t temp;
  temp = a;
  a = b;
  b = temp;
}

void led__fill_screen(uint32_t color) {
  uint32_t c;
  c = pass_through_flag ? pass_through_color : color;
  for (int i = 0; i < number_of_led; i++) {
    led__set_color(i, c);
  }
}

void led__clear() { memset(pixels, 0, number_of_bytes); }
