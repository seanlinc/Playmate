/*********************************************************
 * This is a driver for VS1053 Codec board with amplifier
 * Designed for SJSU SJ2 dev board
 *
 * Written by Yucheng Lin(Sean), Bo An, and Alex Chavez
 *
 *********************************************************/

#include "mp3_vs1053.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "delay.h"
#include "ff.h"
#include "gpio.h"
#include "gpio_lab.h"
#include "queue.h"
#include "ssp2.h"

// static QueueHandle_t song_name_queue;
// static QueueHandle_t song_data_queue;

// static QueueHandle_t Q_song_name;
// static QueueHandle_t Q_song_data;

static gpio_s RST, DREQ, XDCS, MP3CS;

void mp3__set_xdcs() { gpio__set(XDCS); }
void mp3__reset_xdcs() { gpio__reset(XDCS); }

bool data_request() { return gpio__get(DREQ); }

void mp3__sci_write(uint8_t address, uint16_t data) {
  ssp2__initialize(24 * 100);
  while (!data_request()) {
    delay__ms(5);
  }
  gpio__reset(MP3CS);
  ssp2__exchange_byte(VS1053_SCI_WRITE);
  ssp2__exchange_byte(address);
  ssp2__exchange_byte(data >> 8);
  ssp2__exchange_byte(data & 0xFF);
  gpio__set(MP3CS);
}

uint16_t mp3__sci_read(uint8_t address) {
  // printf("sci read\n");
  ssp2__initialize(24 * 100);
  uint16_t data;
  while (!data_request()) {
    delay__ms(5);
  }
  gpio__reset(MP3CS);
  ssp2__exchange_byte(VS1053_SCI_READ);
  ssp2__exchange_byte(address);
  // delay__us(10);
  // printf("sci read send address\n");

  data = ssp2__exchange_byte(0x00);
  data <<= 8;
  data |= ssp2__exchange_byte(0x00);

  // printf("sci read data\n");
  gpio__set(MP3CS);

  // printf("sci read finished\n");
  return data;
}

bool mp3__init() {

  printf("Enter mp3 init\n");
  XDCS = gpio__construct_with_function(2, 1, GPIO__FUNCTION0);
  gpio__set_as_output(XDCS);

  DREQ = gpio__construct_with_function(2, 4, GPIO__FUNCTION0);
  gpio__set_as_input(DREQ);

  MP3CS = gpio__construct_with_function(2, 6, GPIO__FUNCTION0);
  gpio__set_as_output(MP3CS);

  RST = gpio__construct_with_function(1, 29, GPIO__FUNCTION0);
  gpio__set_as_output(RST);

  // gpio__reset(RST);
  gpio__set(MP3CS);
  gpio__set(XDCS);

  printf("Pin init finished\n");

  ssp2__initialize(8 * 1000);

  // mp3__sci_write(VS1053_REG_CLOCKF, 0x2000);

  printf("SSP2 init finished\n");

  // mp3__sci_write(VS1053_REG_CLOCKF, 0x2000);
  // mp3__sci_write(VS1053_REG_VOLUME, 0x3838);

  mp3__hardware_reset();
  // printf("mp3 reset finished\n");

  printf("sci_read value: 0x%04x\n", mp3__sci_read(VS1053_REG_STATUS));
  mp3__sci_write(VS1053_REG_MODE, VS1053_MODE_SM_SDINEW);

  printf("SCI Mode: 0x%04x\n", mp3__sci_read(VS1053_REG_MODE));

  // VS1053 version B7:B3 = 4, otherwise it is not VS1053
  return (mp3__sci_read(VS1053_REG_STATUS) >> 4) & 0x0F;
}

void mp3__software_reset() {
  mp3__sci_write(VS1053_REG_MODE, VS1053_MODE_SM_SDINEW | VS1053_MODE_SM_RESET);
  delay__ms(100);
}

void mp3__hardware_reset() {
  // Using reset pin on the board
  gpio__reset(RST);
  delay__ms(100);
  gpio__set(RST);

  if (!data_request()) {
    printf("Starting reset\n");
    while (!data_request())
      ;
    printf("Reset successful!\n");
  }

  gpio__set(MP3CS);
  gpio__set(XDCS);

  delay__ms(100);

  mp3__software_reset();

  mp3__sci_write(VS1053_REG_CLOCKF, 0xC000);
  mp3__set_volume(50, 50);
}

void mp3__set_volume(uint8_t left, uint8_t right) {
  uint16_t volume;
  volume = left;
  volume <<= 8;
  volume |= right;
  mp3__sci_write(VS1053_REG_VOLUME, volume);
}

void mp3__sine_test(uint8_t n, uint16_t time_ms) {
  mp3__hardware_reset();

  // uint16_t mode = mp3__sci_read(VS1053_REG_STATUS);
  // mode |= 0x0020;

  // mp3__sci_write(VS1053_REG_MODE, mode);

  while (!data_request()) {
    // delay__ms(5);
  }
  mp3__sci_write(VS1053_REG_MODE, 0x0820);

  gpio__reset(XDCS);

  ssp2__exchange_byte(0x53);
  ssp2__exchange_byte(0xEF);
  ssp2__exchange_byte(0x6E);
  ssp2__exchange_byte(n);
  ssp2__exchange_byte(0x00);
  ssp2__exchange_byte(0x00);
  ssp2__exchange_byte(0x00);
  ssp2__exchange_byte(0x00);
  gpio__set(XDCS);

  delay__ms(500);

  gpio__reset(XDCS);

  ssp2__exchange_byte(0x45);
  ssp2__exchange_byte(0x78);
  ssp2__exchange_byte(0x69);
  ssp2__exchange_byte(0x74);
  ssp2__exchange_byte(0x00);
  ssp2__exchange_byte(0x00);
  ssp2__exchange_byte(0x00);
  ssp2__exchange_byte(0x00);

  gpio__set(XDCS);
  printf("Finished sine test\n");
}

// void mp3__send_data_block(mp3_data_block_t data) {
//   UINT bytes_send = 0;

//   while (bytes_send < 512) {
//     while (!data_request())
//       ;
//     // Enable SDI transfer
//     mp3__reset_xdcs();
//     // fprintf(stderr, "Start playing\n");
//     for (UINT byte = bytes_send; byte < (bytes_send + 32); byte++) {
//       // mp3_spi_send_32_bytes(mp3_data[byte]);
//       ssp2__exchange_byte(data[byte]);
//       printf("%02x", data[byte]);
//       printf("Finish one 32 block\n");
//     }
//     printf("Bytes send: %d\n", bytes_send);
//     bytes_send += 32;

//     // vTaskDelay(5);
//     mp3__set_xdcs();
//   }
// }