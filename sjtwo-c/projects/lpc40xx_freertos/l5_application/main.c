#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FreeRTOS.h"
#include "lpc40xx.h"
#include "task.h"

#include "adc.h"
#include "board_io.h"
#include "cli_handlers.h"
#include "common_macros.h"
#include "delay.h"
#include "gpio.h"
#include "gpio_lab.h"
#include "sj2_cli.h"
#include "ssp2.h"

#include "app_cli.h"
#include "event_groups.h"
#include "ff.h"
#include "led_matrix.h"
#include "mp3_vs1053.h"
#include "peripherals_init.h"
#include "queue.h"

static QueueHandle_t Q_song_name;
static QueueHandle_t Q_song_data;

static char song_list[32][128];
static size_t number_of_songs;

void mp3_reader(void *p);
void mp3_player(void *p);
void read_dir();

app_cli_status_e cli__mp3_control(app_cli__argument_t argument, sl_string_t user_input_minus_command_name,
                                  app_cli__print_string_function cli_output) {

  mp3_song_name_t song_name = {0};

  const bool play = sl_string__begins_with_ignore_case(user_input_minus_command_name, "play");
  const bool stop = sl_string__begins_with_ignore_case(user_input_minus_command_name, "stop");
  const bool sinetest = sl_string__begins_with_ignore_case(user_input_minus_command_name, "sine");

  sl_string__erase_first_word(user_input_minus_command_name, ' ');

  if (play) {
    strcpy(song_name, user_input_minus_command_name);
    printf("Song name: %s\n", song_name);
    xQueueSend(Q_song_name, &song_name, portMAX_DELAY);
  } else if (stop) {
    vTaskSuspend(mp3_player);
  } else if (sinetest) {
    mp3__init();
    mp3__sine_test(8, 1000);
  }

  return APP_CLI_STATUS__SUCCESS;
}

gpio_s left_channel, right_channel, strobe, spectrum_reset;

void spectrum__init();
void spectrum__read_frequency();
uint16_t left_frequency[7];
uint16_t right_frequency[7];

int main(void) {

  sj2_cli__init();
  puts("Starting RTOS");

  led__init();
  led__clear();

  // spectrum__init();

  // while (1) {
  //   // spectrum__read_frequency();

  //   // for (int i = 0; i < 7; i++) {
  //   //   printf("Left Frequency[%d] = %d\n", i + 1, left_frequency[i]);
  //   //   printf("Right Frequency[%d] = %d\n", i + 1, right_frequency[i]);
  //   // }
  //   delay__ms(2000);
  // }

  // printf("Number of bytes %d\n", number_of_bytes);
  // led__set_brightness(40);

  while (1) {
    for (int i = 0; i < number_of_led; i++) {
      led__set_brightness(20);
      led__set_pass_through_color(led__change_color(177, 43, 186));
      led__draw_pixel(rand() % 16, rand() % 8, (uint32_t)0);
      led__clean_pass_through();
      led__show();
      delay__ms(200);
      led__clear();
    }
  }

  // for (int b = 0; b < 3; b++) { //  'b' counts from 0 to 2...
  //   led__clear();               //   Set all pixels in RAM to 0 (off)
  //   // 'c' counts up from 'b' to end of strip in increments of 3...
  //   for (int c = b; c < number_of_led; c += 3) {
  //     // hue of pixel 'c' is offset by an amount to make one full
  //     // revolution of the color wheel (range 65536) along the length
  //     // of the strip (strip.numPixels() steps):
  //     led__set_color(c, led__change_color(177, 43, 186));
  //   }
  //   led__show(); // Update strip with new contents
  //   delay__ms(300);
  // }
  // }

  // read_dir();

  // for (size_t i = 0; i < number_of_songs; i++) {
  //   printf("%2d: %s\n", (1 + i), song_list[i]);
  // }

  // Q_song_name = xQueueCreate(1, sizeof(mp3_song_name_t));
  // Q_song_data = xQueueCreate(1, sizeof(mp3_data_block_t));

  // if (!mp3__init()) {
  //   printf("Can't find VS1053 decoder\n");
  // } else {
  //   printf("VS1053 initialize successfully\n");
  //   // mp3__sine_test(3, 100);
  // }

  // mp3__software_reset();

  // mp3__sci_write(VS1053_REG_MODE, VS1053_MODE_SM_SDINEW);
  // printf("CLOCKF: 0x%04x\n", mp3__sci_read(VS1053_REG_CLOCKF));

  // ssp2__initialize(8 * 1000);

  // vTaskDelay(100);
  // mp3__sine_test(8, 1000);
  // delay__ms(100);

  // xTaskCreate(mp3_reader, "Mp3 Reader", 2000 + sizeof(mp3_data_block_t), NULL, 1, NULL);
  // xTaskCreate(mp3_player, "Mp3 Player", 2000 + sizeof(mp3_data_block_t), NULL, 2, NULL);

  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}

void spectrum__init() {

  left_channel = gpio__construct_with_function(1, 30, GPIO__FUNCTION3);  // ADC4
  right_channel = gpio__construct_with_function(1, 31, GPIO__FUNCTION3); // ADC5
  // strobe = gpio__construct_with_function(1, 20, GPIO__FUNCTION0);
  // spectrum_reset = gpio__construct_with_function(1, 28, GPIO__FUNCTION0);

  strobe = gpio__construct_as_output(1, 20);
  spectrum_reset = gpio__construct_as_output(1, 28);

  LPC_IOCON->P1_31 &= ~(1 << 7);
  LPC_IOCON->P1_30 &= ~(1 << 7);

  gpio__set_as_output(strobe);
  gpio__set_as_output(spectrum_reset);

  adc__initialize();
}

void spectrum__read_frequency() {

  gpio__set(spectrum_reset);
  delay__us(1);
  gpio__reset(spectrum_reset);
  delay__us(100);

  for (int i = 0; i < 7; i++) {
    gpio__set(strobe);
    delay__us(50);
    gpio__reset(strobe);
    delay__us(50);
    left_frequency[i] = adc__get_adc_value(ADC__CHANNEL_4);
    right_frequency[i] = adc__get_adc_value(ADC__CHANNEL_5);
  }
}

void read_dir() {
  FRESULT result;
  static FILINFO fno;
  const char *root_path = "/";
  DIR dir;

  result = f_opendir(&dir, root_path);
  if (result == FR_OK) {
    for (;;) {
      result = f_readdir(&dir, &fno);
      if (result != FR_OK || fno.fname[0] == 0) {
        break; // Break in error or end of dir
      }
      if (fno.fattrib & AM_DIR) // It is a directory
      {
        // Any directory will be skipped for now
      } else {
        // printf("%s/%s\n", root_path, fno.fname);
        if (mp3__is_mp3_file(fno.fname)) {
          snprintf(song_list[number_of_songs], sizeof(song_list[number_of_songs]), "%.127s", fno.fname);
          ++number_of_songs;
        } else {
        }
      }
    }
    f_closedir(&dir);
  }
}

void mp3_reader(void *p) {

  FIL file;
  mp3_song_name_t song_name = "Old Dominion.mp3";
  mp3_data_block_t mp3_data;
  UINT br;

  while (1) {
    if (xQueueReceive(Q_song_name, &song_name, portMAX_DELAY)) {

      printf("Got song name from queue: %s\n", song_name);
      FRESULT result = f_open(&file, song_name, FA_READ);

      if (FR_OK == result) {
        fprintf(stderr, "Opening song: %s\n", song_name);
        while (1) {
          f_read(&file, &mp3_data, sizeof(mp3_data), &br);
          if (br == 0) {
            printf("File read error\n");
            break;
          }
          // printf("Sending to queue\n");
          xQueueSend(Q_song_data, (void *)mp3_data, portMAX_DELAY);
          // xQueueSend(Q_song_data, &mp3_data, portMAX_DELAY);
          // xQueueSend(Q_song_data, &mp3_data, portMAX_DELAY);
          if (uxQueueMessagesWaiting(Q_song_name)) {
            break;
          }
        }
        // printf("File read error: %d\n", status);
        printf("End of mp3 file\n");

      } else {
        fprintf(stderr, "Failed to open file\n");
        f_close(&file);
      }
    }
  }
}

void mp3_player(void *p) {

  mp3_data_block_t mp3_data;
  // size_t bytes_send = 0; // Will use it for stop and resume a song
  int position = 0;
  while (1) {
    if (xQueueReceive(Q_song_data, &mp3_data, portMAX_DELAY)) {

      mp3__send_data_block(mp3_data);

      // ssp2__initialize(8 * 1000);
      // bytes_send = 0;

      // while (bytes_send < sizeof(mp3_data)) {

      //   while (!data_request())
      //     ;
      //   // Enable SDI transfer
      //   mp3__reset_xdcs();
      //   // fprintf(stderr, "Start playing\n");
      //   for (size_t byte = bytes_send; byte < (bytes_send + 32); byte++) {
      //     ssp2__exchange_byte(mp3_data[byte]);
      //     // printf("%02x", mp3_data[byte]);
      //   }
      //   // printf("Finish one 32 block\n");
      //   // printf("Bytes send: %d\n", bytes_send);
      //   bytes_send += 32;
      //   // vTaskDelay(5);
      //   mp3__set_xdcs();
      // }
      printf("Position: %d\n", position);
      position += 512;
    }
  }
}
