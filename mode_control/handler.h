#ifndef HANDLER_H
#define HANDLER_H

#include <DueTimer.h>
#include "pin_definitions.h"
#include "data.h"

#define MODE_0 0
#define MODE_1 1
#define MODE_2 2
#define MODE_3 3
#define MODE_4 4
#define MODE_5 5
#define MODE_6 6
#define MODE_7 7
#define MODE_8 8
#define MODE_9 9
#define MODE_10 10
#define MODE_11 11
#define MODE_12 12
#define MODE_13 13
#define MODE_14 14
#define MODE_15 15

#define INIT 0
#define DATA_OUTPUT_START 1
#define DATA_OUTPUT_END 2

static uint8_t data_output_state = INIT;
static uint32_t cur_bit_idx = 0;

typedef void (*mode_handler_t)();

static mode_handler_t current_handler = NULL;
static void start_timer_1(uint16_t bit_interval_us);
static void timer_1_handler();

void register_handler(mode_handler_t h) {
  current_handler = h;
}

void run_handler() {
  if (current_handler)
    current_handler();
}

// handlers
void mode_0_handler() {
  digitalWrite(MOD_PIN, LOW);
  digitalWrite(CONTROL_PIN, HIGH);
}

void mode_1_handler() {
  digitalWrite(MOD_PIN, HIGH);
  digitalWrite(CONTROL_PIN, LOW);
}

void mode_2_handler() {
  start_timer_1(BIT_INTERVAL_US_1);

  while (data_output_state == DATA_OUTPUT_START) {
    digitalWrite(LED_BUILTIN, HIGH);
  }

  while (1) {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void mode_3_handler() {
  start_timer_1(BIT_INTERVAL_US_1);

  while (1) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void mode_4_handler() {
  start_timer_1(BIT_INTERVAL_US_2);

  while (data_output_state == DATA_OUTPUT_START) {
    digitalWrite(LED_BUILTIN, HIGH);
  }

  while (1) {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void mode_5_handler() {
  start_timer_1(BIT_INTERVAL_US_2);

  while (1) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void mode_6_handler() {
  start_timer_1(BIT_INTERVAL_US_3);

  while (data_output_state == DATA_OUTPUT_START) {
    digitalWrite(LED_BUILTIN, HIGH);
  }

  while (1) {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void mode_7_handler() {
  start_timer_1(BIT_INTERVAL_US_3);

  while (1) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void mode_8_handler() {
  return;
}

void mode_9_handler() {
  return;
}

void mode_10_handler() {
  return;
}

void mode_11_handler() {
  return;
}

void mode_12_handler() {
  return;
}
void mode_13_handler() {
  return;
}
void mode_14_handler() {
  return;
}
void mode_15_handler() {
  return;
}

void error_handler() {
  while (1) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
}

static void start_timer_1(uint16_t bit_interval_us) {
  cur_bit_idx = 0;
  data_output_state = DATA_OUTPUT_START;

  Timer1.attachInterrupt(timer_1_handler);
  Timer1.setPeriod(bit_interval_us);
  Timer1.start();
}

static void timer_1_handler() {
  if (data_output_state == DATA_OUTPUT_END) {
    return;
  }

  uint8_t data_out = (uint8_t)(rand_data[cur_bit_idx] & 0x01);

  digitalWrite(MOD_PIN, data_out ? HIGH : LOW);
  digitalWrite(CONTROL_PIN, data_out ? LOW : HIGH);

  cur_bit_idx++;

  if (cur_bit_idx >= (uint32_t)BIT_NO) {
    data_output_state = DATA_OUTPUT_END;
  }
}

#endif