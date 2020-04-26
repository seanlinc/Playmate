/*********************************************************
 * This is a driver for SSD1306 OLED display
 * Designed for SJSU SJ2 dev board
 *
 * Written by Yucheng Lin(Sean), Bo An, and Alex Chavez
 *
 *********************************************************/
#include "oled_ssd1306.h"

#include "glcdfont.c"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// SJ2 libraries
#include "delay.h"
#include "gpio.h"
// #include "splash.h"
#include "clock.h"
#include "i2c.h"
#include "ssp2.h"

#define ssd1306_swap(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) ///< No-temp-var swap operation
#define abs(x) ((x) > 0 ? (x) : -(x))
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))

// RST is active low
gpio_s SCL, SDA, DC, RST;

static const uint8_t oled_address = 0x78;

void oled__constructor(int16_t w, int16_t h) {
  _width = WIDTH = w;
  _height = HEIGHT = h;

  rotation = 0;
  cursor_y = cursor_x = 0;
  textsize = 1;
  textcolor = textbgcolor = 0xFFFF;
  wrap = true;
}

void oled__peripheral_init() {
  // CS = gpio__construct(2, 1);
  // SDA = gpio__construct_with_function(0, 10, GPIO__FUNCTION_2);
  // SCL = gpio__construct_with_function(0, 11, GPIO__FUNCTION_2);
  // DC = gpio__construct(2, 4);
  RST = gpio__construct(2, 6);

  const uint32_t i2c_speed_hz = UINT32_C(400) * 1000;
  i2c__initialize(I2C__2, i2c_speed_hz, clock__get_peripheral_clock_hz());

  // gpio__set_as_output(CS);
  // gpio__set_as_output(DC);
  gpio__set_as_output(RST);
  gpio__set(RST);

  // ssp2__initialize(8 * 1000);
}

bool oled__init() {

  printf("Enter oled init\n");
  // Peripheral initialize
  delay__ms(100); // Wait VCC rise to 3.3V

  oled__peripheral_init();

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

  oled__send_command(OLED_HEIGHT - 1); // 0x3F

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
  oled__send_command(SSD1306_DEACTIVATE_SCROLL);
  oled__send_command(SSD1306_DISPLAYON); // Turn on OLED
  oled__fillAll();
  delay__ms(2000);
  // oled__clear_display();
  // oled__clear();
  puts("OLED initilize successfully!");
  return true;
}

void oled__send_command(uint8_t command) {
  // gpio__reset(CS);
  // gpio__reset(DC);
  // ssp2__exchange_byte(command);
  // gpio__set(CS);
  i2c__write_single(I2C__2, oled_address, (uint8_t)0x00, command);
}

void oled__send_data(uint8_t data) {
  // gpio__reset(CS);
  // gpio__set(DC);
  // ssp2__exchange_byte(data);
  // gpio__set(CS);
  i2c__write_single(I2C__2, oled_address, (uint8_t)0x40, data);
}
void oled__clear_display() { memset(oled_buffer, 0, (OLED_WIDTH * (OLED_HEIGHT + 7) / 8)); }

void oled__display() {
  // gpio__reset(CS);
  // gpio__reset(DC);

  oled__send_command(SSD1306_PAGEADDR);
  oled__send_command(0);    // Page start address
  oled__send_command(0xFF); // Page end address
  oled__send_command(SSD1306_COLUMNADDR);
  oled__send_command(0); // Column start address

  oled__send_command(OLED_WIDTH - 1); // Column end address

  uint16_t count = OLED_WIDTH * ((OLED_HEIGHT + 7) / 8);
  uint8_t *ptr = oled_buffer;

  while (count--) {
    oled__send_data(*ptr++);
  }
}

void oled__fill(uint8_t fill_data) {
  uint8_t page_count, col_count;
  // oled__send_command(SSD1306_PAGEADDR);
  // oled__send_command(0x00); // Page start address
  // oled__send_command(0xFF); // Page end address
  // oled__send_command(SSD1306_COLUMNADDR);
  for (page_count = 0; page_count < 8; page_count++) {
    oled__send_command(0xb0 + page_count);
    oled__send_command(0x00);
    oled__send_command(0x10);
    for (col_count = 0; col_count < 127; col_count++) {
      oled__send_data(fill_data);
    }
  }
}
void oled__refresh(uint8_t buffer) {
  uint8_t i, n;
  uint8_t *ptr = oled_buffer;
  for (i = 0; i < 8; i++) {
    oled__send_command(0xb0 + i);
    oled__send_command(0x00);
    oled__send_command(0x10);
    for (n = 0; n < 128; n++) {
      oled__send_data(*ptr++);
    }
  }
}
void oled__clear() { oled__fill(0x00); }
void oled__fillAll() { oled__fill(0x01); }
void oled__on() {
  oled__send_command(0x8D);
  oled__send_command(0x14);
  oled__send_command(SSD1306_DISPLAYON);
}
void oled__off() {
  oled__send_command(0x8D);
  oled__send_command(0x10);
  oled__send_command(SSD1306_DISPLAYOFF);
}
void oled__set_pos(uint8_t x, uint8_t y) {
  oled__send_command(0xb0 + y);
  oled__send_command((x & 0xf0) >> 4 | 0x10);
  oled__send_command((x & 0xf0) >> 4 | 0x10);
}

void oled__drawPixel(int16_t x, int16_t y, uint16_t color) {

  if ((x >= 0) && (x < OLED_WIDTH) && (y >= 0) && (y < OLED_HEIGHT)) {
    // Pixel is in-bounds. Rotate coordinates if needed.
    switch (oled__getRotation()) {
    case 1:
      ssd1306_swap(x, y);
      x = WIDTH - x - 1;
      break;
    case 2:
      x = WIDTH - x - 1;
      y = HEIGHT - y - 1;
      break;
    case 3:
      ssd1306_swap(x, y);
      y = HEIGHT - y - 1;
      break;
    }
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

uint8_t oled__getRotation(void) {
  rotation %= 4;
  return rotation;
}

void oled__setRotation(uint8_t x) {
  x %= 4; // cant be higher than 3
  rotation = x;
  switch (x) {
  case 0:
  case 2:
    _width = WIDTH;
    _height = HEIGHT;
    break;
  case 1:
  case 3:
    _width = HEIGHT;
    _height = WIDTH;
    break;
  }
}

void oled__drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    if (cornername & 0x4) {
      oled__drawPixel(x0 + x, y0 + y, color);
      oled__drawPixel(x0 + y, y0 + x, color);
    }
    if (cornername & 0x2) {
      oled__drawPixel(x0 + x, y0 - y, color);
      oled__drawPixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
      oled__drawPixel(x0 - y, y0 + x, color);
      oled__drawPixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
      oled__drawPixel(x0 - y, y0 - x, color);
      oled__drawPixel(x0 - x, y0 - y, color);
    }
  }
}

void oled__fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
  oled__drawFastVLine(x0, y0 - r, 2 * r + 1, color);
  oled__fillCircleHelper(x0, y0, r, 3, 0, color);
}

// used to do circles and roundrects!
void oled__fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color) {

  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    if (cornername & 0x1) {
      oled__drawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
      oled__drawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
    }
    if (cornername & 0x2) {
      oled__drawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
      oled__drawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
    }
  }
}

void oled__drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  oled__drawPixel(x0, y0 + r, color);
  oled__drawPixel(x0, y0 - r, color);
  oled__drawPixel(x0 + r, y0, color);
  oled__drawPixel(x0 - r, y0, color);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    oled__drawPixel(x0 + x, y0 + y, color);
    oled__drawPixel(x0 - x, y0 + y, color);
    oled__drawPixel(x0 + x, y0 - y, color);
    oled__drawPixel(x0 - x, y0 - y, color);
    oled__drawPixel(x0 + y, y0 + x, color);
    oled__drawPixel(x0 - y, y0 + x, color);
    oled__drawPixel(x0 + y, y0 - x, color);
    oled__drawPixel(x0 - y, y0 - x, color);
  }
}
void oled__drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    ssd1306_swap(x0, y0);
    ssd1306_swap(x1, y1);
  }

  if (x0 > x1) {
    ssd1306_swap(x0, x1);
    ssd1306_swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      oled__drawPixel(y0, x0, color);
    } else {
      oled__drawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

void oled__drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  oled__drawFastHLine(x, y, w, color);
  oled__drawFastHLine(x, y + h - 1, w, color);
  oled__drawFastVLine(x, y, h, color);
  oled__drawFastVLine(x + w - 1, y, h, color);
}

void oled__drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  // stupidest version - update in subclasses if desired!
  oled__drawLine(x, y, x, y + h - 1, color);
}

void oled__drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  // stupidest version - update in subclasses if desired!
  oled__drawLine(x, y, x + w - 1, y, color);
}

void oled__fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  // stupidest version - update in subclasses if desired!
  for (int16_t i = x; i < x + w; i++) {
    oled__drawFastVLine(i, y, h, color);
  }
}

void oled__fillScreen(uint16_t color) { oled__fillRect(0, 0, OLED_WIDTH, OLED_HEIGHT, color); }

void oled__drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) {
  // smarter version
  oled__drawFastHLine(x + r, y, w - 2 * r, color);         // Top
  oled__drawFastHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
  oled__drawFastVLine(x, y + r, h - 2 * r, color);         // Left
  oled__drawFastVLine(x + w - 1, y + r, h - 2 * r, color); // Right
  // draw four corners
  oled__drawCircleHelper(x + r, y + r, r, 1, color);
  oled__drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
  oled__drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
  oled__drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

void oled__fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) {
  // smarter version
  oled__fillRect(x + r, y, w - 2 * r, h, color);

  // draw four corners
  oled__fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
  oled__fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}

void oled__drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
  oled__drawLine(x0, y0, x1, y1, color);
  oled__drawLine(x1, y1, x2, y2, color);
  oled__drawLine(x2, y2, x0, y0, color);
}

void oled__fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {

  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    ssd1306_swap(y0, y1);
    ssd1306_swap(x0, x1);
  }
  if (y1 > y2) {
    ssd1306_swap(y2, y1);
    ssd1306_swap(x2, x1);
  }
  if (y0 > y1) {
    ssd1306_swap(y0, y1);
    ssd1306_swap(x0, x1);
  }

  if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if (x1 < a)
      a = x1;
    else if (x1 > b)
      b = x1;
    if (x2 < a)
      a = x2;
    else if (x2 > b)
      b = x2;
    oled__drawFastHLine(a, y0, b - a + 1, color);
    return;
  }

  int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0, dx12 = x2 - x1, dy12 = y2 - y1, sa = 0,
          sb = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if (y1 == y2)
    last = y1; // Include y1 scanline
  else
    last = y1 - 1; // Skip it

  for (y = y0; y <= last; y++) {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b)
      ssd1306_swap(a, b);
    oled__drawFastHLine(a, y, b - a + 1, color);
  }
  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for (; y <= y2; y++) {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b)
      ssd1306_swap(a, b);
    oled__drawFastHLine(a, y, b - a + 1, color);
  }
}

void oled__drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) {

  int16_t i, j, byteWidth = (w + 7) / 8;

  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++) {
      if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
        oled__drawPixel(x + i, y + j, color);
      }
    }
  }
}

void oled__write(uint8_t c) {
  if (c == '\n') {
    cursor_y += textsize * 8;
    cursor_x = 0;
  } else if (c == '\r') {
    // skip em
  } else {
    oled__drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
    cursor_x += textsize * 6;
    if (wrap && (cursor_x > (_width - textsize * 6))) {
      cursor_y += textsize * 8;
      cursor_x = 0;
    }
  }
}

void oled__drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size) {

  if ((x >= OLED_WIDTH) ||        // Clip right
      (y >= OLED_HEIGHT) ||       // Clip bottom
      ((x + 5 * size - 1) < 0) || // Clip left
      ((y + 8 * size - 1) < 0))   // Clip top
    return;

  for (int8_t i = 0; i < 6; i++) {
    uint8_t line;
    if (i == 5)
      line = 0x0;
    else
      line = pgm_read_byte(font + (c * 5) + i);
    for (int8_t j = 0; j < 8; j++) {
      if (line & 0x1) {
        if (size == 1) // default size
          oled__drawPixel(x + i, y + j, color);
        else { // big size
          oled__fillRect(x + (i * size), y + (j * size), size, size, color);
        }
      } else if (bg != color) {
        if (size == 1) // default size
          oled__drawPixel(x + i, y + j, bg);
        else { // big size
          oled__fillRect(x + i * size, y + j * size, size, size, bg);
        }
      }
      line >>= 1;
    }
  }
}
void oled__writeString(char *input, uint8_t count) {
  int8_t x = 0, y = 0;
  char *ptr = input;
  while (count--) {
    oled__drawChar(x, y, *ptr++, SSD1306_WHITE, SSD1306_BLACK, 1);
    x = x + 6;
    if (x >= cursor_x) {
      y = y + 8;
      x = 0;
    }
  }
}
void oled__drawbutton(uint8_t x, uint8_t y, uint8_t color) { oled__drawChar(x, y, '<', color, SSD1306_BLACK, 1); }

void oled__setCursor(int16_t x, int16_t y) {
  cursor_x = x;
  cursor_y = y;
}

void oled__setTextSize(uint8_t s) { textsize = (s > 0) ? s : 1; }

void oled__setTextColor(uint16_t c) {
  textcolor = c;
  textbgcolor = c;
  // for 'transparent' background, we'll set the bg
  // to the same as fg instead of using a flag
}

void oled__setTextColor_b(uint16_t c, uint16_t b) {
  textcolor = c;
  textbgcolor = b;
}

void oled__setTextWrap(bool w) { wrap = w; }

void oled__startscroll_right_All() {

  oled__send_command(SSD1306_RIGHT_HORIZONTAL_SCROLL); // send command

  oled__send_command(0x00); // set A[7:0] dummy byte
  oled__send_command(0x00); // set starting page
  oled__send_command(0x00); // set time interval
  oled__send_command(0xFF); // define end page address

  oled__send_command(0x00); // e[7:0 ] dummy byte
  oled__send_command(0x00); // set starting col
  oled__send_command(0xFF); // set end cool
  oled__send_command(SSD1306_ACTIVATE_SCROLL);
}

void oled__startscroll_right(page_t start_Page, page_t stop_Page, uint8_t star_Col, uint8_t end_Col,
                             FRAME_FREQUENCY_T frequency) {

  oled__send_command(SSD1306_RIGHT_HORIZONTAL_SCROLL); // send command

  oled__send_command(0x00);       // set A[7:0] dummy byte
  oled__send_command(start_Page); // set starting page
  oled__send_command(frequency);  // set time interval
  oled__send_command(stop_Page);  // define end page address

  oled__send_command(0x00);     // e[7:0 ] dummy byte
  oled__send_command(star_Col); // set starting col
  oled__send_command(end_Col);  // set end cool
  oled__send_command(SSD1306_ACTIVATE_SCROLL);
}

void oled__startscroll_left_All() {

  oled__send_command(SSD1306_LEFT_HORIZONTAL_SCROLL); // send command

  oled__send_command(0x00); // set A[7:0] dummy byte
  oled__send_command(0x00); // set starting page
  oled__send_command(0x00); // set time interval
  oled__send_command(0xFF); // define end page address

  oled__send_command(0x00); // e[7:0 ] dummy byte
  oled__send_command(0x00); // set starting col
  oled__send_command(0xFF); // set end cool
  oled__send_command(SSD1306_ACTIVATE_SCROLL);
}

void oled__startscroll_left(page_t start_Page, page_t stop_Page, uint8_t star_Col, uint8_t end_Col,
                            FRAME_FREQUENCY_T frequency) {

  oled__send_command(SSD1306_LEFT_HORIZONTAL_SCROLL); // send command

  oled__send_command(0x00);       // set A[7:0] dummy byte
  oled__send_command(start_Page); // set starting page
  oled__send_command(frequency);  // set time interval
  oled__send_command(stop_Page);  // define end page address

  oled__send_command(0x00);     // e[7:0 ] dummy byte
  oled__send_command(star_Col); // set starting col
  oled__send_command(end_Col);  // set end cool
  oled__send_command(SSD1306_ACTIVATE_SCROLL);
}

void oled__V_and_H_scroll_left(page_t start_Page, page_t stop_Page, uint8_t star_Col, uint8_t end_Col, uint8_t V_offset,
                               FRAME_FREQUENCY_T frequency, bool Hori) {

  oled__send_command(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL); // send command
  if (Hori == true)
    oled__send_command(0x01); // Horizontal scroll
  else
    oled__send_command(0x00);
  oled__send_command(start_Page); // set starting page
  oled__send_command(frequency);  // set time interval
  oled__send_command(stop_Page);  // define end page address

  oled__send_command(V_offset); // vertical scrool offset 01h-3fh
  oled__send_command(star_Col); // set starting col
  oled__send_command(end_Col);  // set end cool
  oled__send_command(SSD1306_ACTIVATE_SCROLL);
}

void oled__V_and_H_scroll_right(page_t start_Page, page_t stop_Page, uint8_t star_Col, uint8_t end_Col,
                                uint8_t V_offset, FRAME_FREQUENCY_T frequency, bool Hori) {

  oled__send_command(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL); // send command
  if (Hori == true)
    oled__send_command(0x01); // Horizontal scroll
  else
    oled__send_command(0x00);
  oled__send_command(start_Page); // set starting page
  oled__send_command(frequency);  // set time interval
  oled__send_command(stop_Page);  // define end page address

  oled__send_command(V_offset); // vertical scrool offset 01h-3fh
  oled__send_command(star_Col); // set starting col
  oled__send_command(end_Col);  // set end cool
  oled__send_command(SSD1306_ACTIVATE_SCROLL);
}
void oled_deActive_Scroll() { oled__send_command(SSD1306_DEACTIVATE_SCROLL); }

void oled__invertDisplay(bool i) { oled__send_command(i ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY); }