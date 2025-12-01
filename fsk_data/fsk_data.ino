#include <DueTimer.h>

#define CONTROL_PIN 7
#define MOD_PIN 6
#define BUTTON_PIN 5

#define SWITCHING (1 << 0)
#define LOW_FREQ_ONLY (1 << 1)
#define HIGH_FREQ_ONLY (1 << 2)

#define READING (1 << 0)
#define WRITING (1 << 1)

uint8_t switching_mode = SWITCHING;
uint8_t state = READING;

volatile uint8_t current_byte = 0;
volatile uint8_t bit_idx = 0;
volatile uint16_t bit_interval_us = 20;  // 100us per bit

void button_isr_handler() {
  if (switching_mode == LOW_FREQ_ONLY) {
    switching_mode = HIGH_FREQ_ONLY;
  } else {
    switching_mode = LOW_FREQ_ONLY;
    // digitalWrite(MOD_PIN, LOW);
    // digitalWrite(CONTROL_PIN, HIGH);
  }
}

void timer_handler() {
  if (state == READING) return;

  uint8_t bit = (current_byte >> (7 - bit_idx)) & 0x01;

  if (bit) {
    digitalWrite(MOD_PIN, HIGH);
    digitalWrite(CONTROL_PIN, LOW);
  } else {
    digitalWrite(MOD_PIN, LOW);
    digitalWrite(CONTROL_PIN, HIGH);
  }

  bit_idx++;

  if (bit_idx >= 8) {
    bit_idx = 0;
    state = READING;  // Back to reading next byte
  }
}

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(MOD_PIN, OUTPUT);
  pinMode(CONTROL_PIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);

  // If the button is HIGH at boot enter  no switching mode
  if (digitalRead(BUTTON_PIN) == HIGH) {
    switching_mode = LOW_FREQ_ONLY;
    attachInterrupt(BUTTON_PIN, button_isr_handler, FALLING);
    // Else enter switching mode
  } else {
    Serial.begin(115200);
    switching_mode = SWITCHING;
    Timer1.attachInterrupt(timer_handler);
    Timer1.setPeriod(bit_interval_us);
    Timer1.start();
  }
}

void loop() {

  while (switching_mode == LOW_FREQ_ONLY || switching_mode == HIGH_FREQ_ONLY) {
    switch (switching_mode) {
      case LOW_FREQ_ONLY:
        digitalWrite(MOD_PIN, LOW);
        digitalWrite(CONTROL_PIN, HIGH);

        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
        break;

      case HIGH_FREQ_ONLY:
        digitalWrite(MOD_PIN, HIGH);
        digitalWrite(CONTROL_PIN, LOW);

        digitalWrite(LED_BUILTIN, HIGH);
        delay(600);
        digitalWrite(LED_BUILTIN, LOW);
        delay(600);
        break;
    }
  }

  while (Serial.available() && state == READING) {
    current_byte = Serial.read();
    state = WRITING;
    bit_idx = 0;
  }
}
