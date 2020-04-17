/*********************************************************
 * This is a driver for VS1053 Codec board with amplifier
 * Designed for SJSU SJ2 dev board
 *
 * Written by Yucheng Lin(Sean), Bo An, and Alex Chavez
 *
 *********************************************************/
#pragma once

#include "gpio.h"
#include <stdbool.h>
#include <stdint.h>

typedef char mp3_song_name_t[32];
typedef uint8_t mp3_data_block_t[512];

#define VS1053_SCI_READ 0x03
#define VS1053_SCI_WRITE 0x02

#define VS1053_REG_MODE 0x00
#define VS1053_REG_STATUS 0x01
#define VS1053_REG_BASS 0x02
#define VS1053_REG_CLOCKF 0x03
#define VS1053_REG_DECODETIME 0x04
#define VS1053_REG_AUDATA 0x05
#define VS1053_REG_WRAM 0x06
#define VS1053_REG_WRAMADDR 0x07
#define VS1053_REG_HDAT0 0x08
#define VS1053_REG_HDAT1 0x09
#define VS1053_REG_VOLUME 0x0B

#define VS1053_GPIO_DDR 0xC017
#define VS1053_GPIO_IDATA 0xC018
#define VS1053_GPIO_ODATA 0xC019

#define VS1053_INT_ENABLE 0xC01A

#define VS1053_MODE_SM_DIFF 0x0001
#define VS1053_MODE_SM_LAYER12 0x0002
#define VS1053_MODE_SM_RESET 0x0004
#define VS1053_MODE_SM_CANCEL 0x0008
#define VS1053_MODE_SM_EARSPKLO 0x0010
#define VS1053_MODE_SM_TESTS 0x0020
#define VS1053_MODE_SM_STREAM 0x0040
#define VS1053_MODE_SM_SDINEW 0x8800 // The default is 0x0800 change bit 15 to 1 for 24Mhz SPI
#define VS1053_MODE_SM_ADPCM 0x1000
#define VS1053_MODE_SM_LINE1 0x4000
#define VS1053_MODE_SM_CLKRANGE 0x8000

#define VS1053_SCI_AIADDR 0x0A
#define VS1053_SCI_AICTRL0 0x0C
#define VS1053_SCI_AICTRL1 0x0D
#define VS1053_SCI_AICTRL2 0x0E
#define VS1053_SCI_AICTRL3 0x0F

#define VS1053_DATABUFFERLEN 32

bool mp3__init();
bool data_request();

void mp3__sci_write(uint8_t address, uint16_t data);
uint16_t mp3__sci_read(uint8_t address);

void mp3__sine_test(uint8_t n, uint16_t time_ms);
void mp3__set_volume(uint8_t left, uint8_t right);
void mp3__software_reset();
void mp3__hardware_reset();
void mp3__play();
void mp3__send_data_block(mp3_data_block_t data);

void mp3__set_xdcs();
void mp3__reset_xdcs();