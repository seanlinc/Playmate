/*********************************************************
 * This is a driver for SSD1306 OLED display
 * Designed for SJSU SJ2 dev board
 *
 * Written by Yucheng Lin(Sean), Bo An, and Alex Chavez
 *
 *********************************************************/

#pragma once

//#include "bitmap.h"
#include "gfxfont.h"
#include "pgmspace.h"
#include "string.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// Define commands
/// fit into the SSD1306_ naming scheme
#define SSD1306_BLACK 0   ///< Draw 'off' pixels
#define SSD1306_WHITE 1   ///< Draw 'on' pixels
#define SSD1306_INVERSE 2 ///< Invert pixels

#define SSD1306_MEMORYMODE 0x20          ///< See datasheet
#define SSD1306_COLUMNADDR 0x21          ///< See datasheet
#define SSD1306_PAGEADDR 0x22            ///< See datasheet
#define SSD1306_SETCONTRAST 0x81         ///< See datasheet
#define SSD1306_CHARGEPUMP 0x8D          ///< See datasheet
#define SSD1306_SEGREMAP 0xA0            ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define SSD1306_DISPLAYALLON 0xA5        ///< Not currently used
#define SSD1306_NORMALDISPLAY 0xA6       ///< See datasheet
#define SSD1306_INVERTDISPLAY 0xA7       ///< See datasheet
#define SSD1306_SETMULTIPLEX 0xA8        ///< See datasheet
#define SSD1306_DISPLAYOFF 0xAE          ///< See datasheet
#define SSD1306_DISPLAYON 0xAF           ///< See datasheet
#define SSD1306_COMSCANINC 0xC0          ///< Not currently used
#define SSD1306_COMSCANDEC 0xC8          ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET 0xD3    ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5  ///< See datasheet
#define SSD1306_SETPRECHARGE 0xD9        ///< See datasheet
#define SSD1306_SETCOMPINS 0xDA          ///< See datasheet
#define SSD1306_SETVCOMDETECT 0xDB       ///< See datasheet

#define SSD1306_SETLOWCOLUMN 0x00  ///< Not currently used
#define SSD1306_SETHIGHCOLUMN 0x10 ///< Not currently used
#define SSD1306_SETSTARTLINE 0x40  ///< See datasheet

#define SSD1306_EXTERNALVCC 0x01  ///< External display voltage source
#define SSD1306_SWITCHCAPVCC 0x02 ///< Gen. display voltage from 3.3V

#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26              ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27               ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A  ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL 0x2E                    ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL 0x2F                      ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3             ///< Set scroll range

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

typedef enum {
  Frame_frequency_5 = 0,
  Frame_frequency_64 = 1,
  Frame_frequency_128 = 2,
  Frame_frequency_256 = 3,
  Frame_frequency_2 = 4,
  Frame_frequency_3 = 5,
  Frame_frequency_4 = 6,
  Frame_frequency_1 = 7,
} FRAME_FREQUENCY_T;
typedef enum {
  page0 = 0,
  page1,
  page2,
  page3,
  page4,
  page5,
  page6,
  page7,

} page_t;
uint8_t *oled_buffer;    // Display buffer
int16_t WIDTH, HEIGHT;   // this is the 'raw' display w/h - never changes
int16_t _width, _height; // dependent on rotation
int16_t cursor_x, cursor_y;
uint16_t textcolor, textbgcolor;
uint8_t textsize;
uint8_t rotation;
int16_t page_start_x, page_start_y;
uint8_t cursor_trace = 0;
uint8_t cursor_position_x = 100;

bool wrap;
void oled__constructor(int16_t w, int16_t h);
void oled__peripheral_init();
bool oled__init();
void oled__display();
void oled__clear_display();
void oled__send_command(uint8_t command);
void oled__drawPixel(int16_t x, int16_t y, uint16_t color);
void oled__draw_line();
void oled__fill(uint8_t fill_data);
void oled__clear();
void oled__fillAll();
void oled__on();
void oled__off();
void oled__refresh(uint8_t buffer);
void oled__set_pos(uint8_t x, uint8_t y);
void oled__refresh(uint8_t buffer);
void oled__setRotation(uint8_t r);
uint8_t oled__getRotation(void);
// void oled__drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
// void oled__drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color);
// void oled__drawFastVLine(int16_t x, int16_t y, int16_t w, uint16_t color);
// void oled__drawFastVLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color);
void oled__drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void oled__drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void oled__drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void oled__drawFastVLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void oled__drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void oled__fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void oled__fillScreen(uint16_t color);
void oled__fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
void oled__fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void oled__drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
void oled__drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void oled__fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void oled__drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void oled__fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void oled__drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
void oled__drawbutton(uint8_t x, uint8_t y, uint8_t color);

// text functions
void oled__write(uint8_t c);
void oled__drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
void oled__setTextColor_b(uint16_t c, uint16_t b);
void oled__setTextColor(uint16_t c);
void oled__setTextSize(uint8_t s);
void oled__setCursor(int16_t x, int16_t y);
void oled__setTextWrap(bool w);
void oled__writeString(char *input, uint8_t count);
// scroll
void oled__startscroll_right_ALL(page_t start, page_t stop);
void oled__startscroll_right(page_t start_Page, page_t stop_Page, uint8_t star_Col, uint8_t end_Col,
                             FRAME_FREQUENCY_T frequency);
void oled__startscroll_left_ALL(page_t start, page_t stop);
void oled__startscroll_left(page_t start_Page, page_t stop_Page, uint8_t star_Col, uint8_t end_Col,
                            FRAME_FREQUENCY_T frequency);
void oled__V_and_H_scroll_left(page_t start_Page, page_t stop_Page, uint8_t star_Col, uint8_t end_Col, uint8_t V_offset,
                               FRAME_FREQUENCY_T frequency, bool Hori);
void oled__V_and_H_scroll_right(page_t start_Page, page_t stop_Page, uint8_t star_Col, uint8_t end_Col,
                                uint8_t V_offset, FRAME_FREQUENCY_T frequency, bool Hori);
void oled_deActive_Scroll();
void oled__invertDisplay(bool i);
void oled__set_music_pageCursor(uint8_t start_x, uint8_t start_y);

void draw_button(cursor_position_y);
void clear_button(cursor_position_y);
void display_music_page(char array[], uint8_t start_x, uint8_t start_y, uint8_t count);

void button_move_up(uint8_t y);
void button_move_down();