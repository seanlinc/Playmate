#include "spectrum.h"
#include "adc.h"
#include "delay.h"
#include "gpio.h"
#include "lpc40xx.h"

gpio_s left_channel, right_channel, strobe, spectrum_reset;
uint16_t left_frequency[7];
uint16_t right_frequency[7];

void spectrum__init() {

  left_channel = gpio__construct_with_function(1, 30, GPIO__FUNCTION_3);  // ADC4
  right_channel = gpio__construct_with_function(1, 31, GPIO__FUNCTION_3); // ADC5

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