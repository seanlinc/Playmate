/*********************************************************
 * This is a driver for SSD1306 OLED display
 * Designed for SJSU SJ2 dev board
 *
 * Written by Yucheng Lin(Sean), Bo An, and Alex Chavez
 *
 *********************************************************/
#include "oled_ssd1306.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// SJ2 libraries
#include "delay.h"
#include "gpio.h"
// #include "splash.h"
#include "ssp2.h"

#define ssd1306_swap(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) ///< No-temp-var swap operation

// For DC pin, low for command and high for data
// CS and RST is active low
gpio_s CS, DC, RST;

bool oled__init() {

  printf("Enter oled init\n");
  // Peripheral initialize
  delay__ms(100); // Wait VCC rise to 3.3V

  CS = gpio__construct(2, 1);
  DC = gpio__construct(2, 4);
  RST = gpio__construct(2, 6);

  gpio__set_as_output(CS);
  gpio__set_as_output(DC);
  gpio__set_as_output(RST);

  ssp2__initialize(8 * 1000);

  oled_buffer = (uint8_t *)malloc(OLED_WIDTH * ((OLED_HEIGHT + 7) / 8));

  printf("Buffer size = %d\n", sizeof(oled_buffer));

  uint8_t comPins = 0x12;
  uint8_t contrast = 0xCF;

  oled__clear_display();

  // First routine
  oled__send_command(SSD1306_DISPLAYOFF);         // 0xAE
  oled__send_command(SSD1306_SETDISPLAYCLOCKDIV); // 0xD5
  oled__send_command(0x80);                       // Ratio 0x80
  oled__send_command(SSD1306_SETMULTIPLEX);       // 0xA8

  oled__send_command(0x3F); // 0xA8

  // Second routine
  oled__send_command(SSD1306_SETDISPLAYOFFSET);   // 0xD3
  oled__send_command(0x0);                        // No offset
  oled__send_command(SSD1306_SETSTARTLINE | 0x0); // Line 0
  oled__send_command(SSD1306_CHARGEPUMP);         // 0x8D
  oled__send_command(0x14);                       // Using internal VCC

  // Third routine
  oled__send_command(SSD1306_MEMORYMODE);     // 0x20
  oled__send_command(0x00);                   // Horizontol offset
  oled__send_command(SSD1306_SEGREMAP | 0x1); // Rotate screen 180 degrees
  oled__send_command(SSD1306_COMSCANDEC);     // Rotate screen 180 degrees

  // Fourth rountine
  oled__send_command(SSD1306_SETCOMPINS);  // 0xDA
  oled__send_command(0x12);                // For 128 x 64 pixels
  oled__send_command(SSD1306_SETCONTRAST); // 0x81
  oled__send_command(0xCF);                // Set contrast

  oled__send_command(SSD1306_SETPRECHARGE); // 0xD9
  oled__send_command(0xF1);

  // Fifth rountine
  oled__send_command(SSD1306_SETVCOMDETECT); // 0xDB
  oled__send_command(0x40);
  oled__send_command(SSD1306_DISPLAYALLON_RESUME); // 0xA4
  oled__send_command(SSD1306_NORMALDISPLAY);       // 0xA6
  // oled__write_command(SSD1306_DEACTIVATE_SCROLL);
  oled__send_command(SSD1306_DISPLAYON); // Turn on OLED

  puts("OLED initilize successfully!");
  return true;
}

void oled__send_command(uint8_t command) {
  gpio__reset(CS);
  gpio__reset(DC);
  ssp2__exchange_byte(command);
  gpio__set(CS);
}

void oled__clear_display() { memset(oled_buffer, 0, (OLED_WIDTH * (OLED_HEIGHT + 7) / 8)); }

void oled__draw_pixel(int16_t x, int16_t y, uint16_t color) {

  if ((x >= 0) && (x < OLED_WIDTH) && (y >= 0) && (y < OLED_HEIGHT)) {
    // Pixel is in-bounds. Rotate coordinates if needed.

    switch (color) {
    case SSD1306_WHITE:
      oled_buffer[x + (y / 8) * OLED_WIDTH] |= (1 << (y & 7));
      break;
    case SSD1306_BLACK:
      oled_buffer[x + (y / 8) * OLED_WIDTH] &= ~(1 << (y & 7));
      break;
    case SSD1306_INVERSE:
      oled_buffer[x + (y / 8) * OLED_WIDTH] ^= (1 << (y & 7));
      break;
    }
  }
}

void oled__display() {
  gpio__reset(CS);
  gpio__reset(DC);

  oled__send_command(SSD1306_PAGEADDR);
  oled__send_command(0);
  oled__send_command(0xFF);
  oled__send_command(SSD1306_COLUMNADDR);
  oled__send_command(0);

  // oled__send_command(OLED_WIDTH - 1); // Column end address

  uint16_t count = OLED_WIDTH * ((OLED_HEIGHT + 7) / 8);
  uint8_t *ptr = oled_buffer;

  gpio__set(DC);
  while (count--) {
    ssp2__exchange_byte(*ptr++);
  }

  gpio__set(CS);
}