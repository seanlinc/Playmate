#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

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
#include "oled_ssd1306.h"
#include "peripherals_init.h"
#include "playmate.h"
#include "queue.h"
#include "splash.h"

static QueueHandle_t Q_song_name;
static QueueHandle_t Q_song_data;

void mp3_reader(void *p);
void mp3_player(void *p);

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

char array[3][12];
gpio0_s sw1 = {0, 29};

static QueueHandle_t button_handler;

void button_task(void *p) {
  int button = 0;
  gpio0_s *switch1 = (gpio0_s *)(p);
  while (1) {
    button = gpio0__get_level(*switch1);
    xQueueSend(button_handler, &button, 0);
  }
}
void recive_task(void *p) {
  int button = 0;
  while (1) {
    if (xQueueReceive(button_handler, &button, 100)) {
      fprintf(stderr, "button value %u", button);
      if (button == 1) {
        button_move_down();
      }
    }
  }
}

int main(void) {

  sj2_cli__init();
  puts("Starting RTOS");
  // button_handler = xQueueCreate(1, sizeof(int));
  // xTaskCreate(button_task, "button_task", (512U * 8) / sizeof(void *), &sw1, 1, NULL);
  // xTaskCreate(recive_task, "recive_task", (512U * 8) / sizeof(void *), NULL, 1, NULL);
  // oled__init();
  // oled__clear_display();
  // oled__drawBitmap((OLED_WIDTH - 122) / 2, (OLED_HEIGHT - 64) / 2, playmate_data, 122, 64, 1);
  // oled__display();
  // delay__ms(5000);
  // oled__clear_display();
  // gpio0__set_as_output(sw1);
  // // oled__clear_display();
  // // delay__ms(1000);
  // // oled__setCursor(100, 0);
  // // char array[] = "ABCEDFGNSCSOISOIFJW";
  // // oled__writeString(array, strlen(array));
  // // oled__display();
  // uint8_t count;
  // uint8_t start_x = 0, start_y = 0;
  // memset(array, '\0', sizeof(array));
  // strcpy(array[0], "ABCDEFG");
  // strcpy(array[1], "1234567");
  // strcpy(array[2], "!@#$)^&");
  // oled__setCursor(90, 0);
  // for (int i = 0; i < 3; i++) {
  //   count = strlen(array[i]);
  //   display_music_page(array[i], start_x, start_y, count);
  //   printf("array %u", i);
  //   start_x = 0;
  //   start_y = start_y + 8;
  // }
  // oled__display();

  led__init();
  led__clear();

  printf("Number of bytes %d\n", number_of_bytes);
  led__set_brightness(40);
  for (int i = 0; i < number_of_led; i++) {
    // led__set_brightness(20);

    // while (1) {
    led__set_color(10, led__change_color(150, 0, 0));
    led__show();
    delay__ms(500);
    led__clear();
  }
  // }

  // Q_song_name = xQueueCreate(1, sizeof(mp3_song_name_t));
  // Q_song_data = xQueueCreate(1, sizeof(mp3_data_block_t));

  // if (!mp3__init()) {
  //   printf("Can't find VS1053 decoder\n");
  // } else {
  //   printf("VS1053 initialize successfully\n");
  //   mp3__sine_test(3, 100);
  // }

  // mp3__software_reset();

  // mp3__sci_write(VS1053_REG_MODE, VS1053_MODE_SM_SDINEW);
  // printf("CLOCKF: 0x%04x\n", mp3__sci_read(VS1053_REG_CLOCKF));

  // ssp2__initialize(8 * 1000);

  // // vTaskDelay(100);
  // // mp3__sine_test(8, 1000);
  // // delay__ms(100);

  // xTaskCreate(mp3_reader, "Mp3 Reader", 2000 + sizeof(mp3_data_block_t), NULL, 1, NULL);
  // xTaskCreate(mp3_player, "Mp3 Player", 2000 + sizeof(mp3_data_block_t), NULL, 2, NULL);

  // vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}

void mp3_reader(void *p) {

  FIL file;
  mp3_song_name_t song_name = "test.mp3";
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
  size_t bytes_send = 0; // Will use it for stop and resume a song
  int position = 0;
  while (1) {
    if (xQueueReceive(Q_song_data, &mp3_data, portMAX_DELAY)) {
      ssp2__initialize(8 * 1000);
      bytes_send = 0;

      while (bytes_send < sizeof(mp3_data)) {

        while (!data_request())
          ;
        // Enable SDI transfer
        mp3__reset_xdcs();
        // fprintf(stderr, "Start playing\n");
        for (size_t byte = bytes_send; byte < (bytes_send + 32); byte++) {
          ssp2__exchange_byte(mp3_data[byte]);
          // printf("%02x", mp3_data[byte]);
        }
        // printf("Finish one 32 block\n");
        // printf("Bytes send: %d\n", bytes_send);
        bytes_send += 32;
        // vTaskDelay(5);
        mp3__set_xdcs();
      }
      printf("Position: %d\n", position);
      position += 512;
    }
  }
}
