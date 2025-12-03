#include <DueTimer.h>

#define INCLUDE_DATA
#define CONTROL_PIN 7
#define MOD_PIN 6
#define BUTTON_PIN 5

#define PORT SerialUSB


#if defined(INCLUDE_DATA)
#define INIT (1 << 0)
#define START (1 << 1)
#define END (1 << 2)
#endif
#define READING (1 << 3)
#define WRITING (1 << 4)
#define INITIATING (1 << 5)
#define NO_SERIAL (1 << 6)


#define LOW_FREQ_ONLY (1 << 0)
#define HIGH_FREQ_ONLY (1 << 1)
#define SWITCHING (1 << 2)

void timer_handler();
void button_isr_handler();

uint8_t switching_mode;
volatile uint8_t current_byte = 0;
#if defined(INCLUDE_DATA)
#include "data.h"
uint8_t state = INIT;
uint32_t bit_idx = 0;
#else
uint8_t state;
volatile uint8_t bit_idx = 0;
uint16_t bit_interval_us;
#endif


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(MOD_PIN, OUTPUT);
  pinMode(CONTROL_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  if (digitalRead(BUTTON_PIN) == HIGH) {
    switching_mode = LOW_FREQ_ONLY;
    state = NO_SERIAL;
    attachInterrupt(BUTTON_PIN, button_isr_handler, FALLING);
  } else {
#if defined(INCLUDE_DATA)
    Timer1.attachInterrupt(timer_handler);
    Timer1.setPeriod(BIT_INTERVAL_US);
    Timer1.start();
#else
    switching_mode = SWITCHING;
    state = INITIATING;
    PORT.begin(115200);
    Timer1.attachInterrupt(timer_handler);
    digitalWrite(MOD_PIN, LOW);
    digitalWrite(CONTROL_PIN, HIGH);
#endif
  }
}

void button_isr_handler() {
  if (switching_mode == LOW_FREQ_ONLY) {
    switching_mode = HIGH_FREQ_ONLY;
  } else {
    switching_mode = LOW_FREQ_ONLY;
  }
}

void timer_handler() {
#if defined(INCLUDE_DATA)
  if (state == END) return;
  
  uint8_t bit = (uint8_t)(rand_data[bit_idx] & 0x01);
  digitalWrite(MOD_PIN, bit ? HIGH : LOW);
  digitalWrite(CONTROL_PIN, bit ? LOW : HIGH);
  bit_idx++;
  if (bit_idx >= (uint32_t)BIT_NO) {
    state = END;
  }
#else
  if (state == READING) return;
  uint8_t bit = (current_byte >> (7 - bit_idx)) & 0x01;

  digitalWrite(MOD_PIN, bit ? HIGH : LOW);
  digitalWrite(CONTROL_PIN, bit ? LOW : HIGH);
  bit_idx++;
  if (bit_idx >= 8) {
    bit_idx = 0;
    state = READING;
  }
#endif
}

void loop() {
  while (state == NO_SERIAL) {
    switch (switching_mode) {
      case LOW_FREQ_ONLY:
        digitalWrite(MOD_PIN, LOW);
        digitalWrite(CONTROL_PIN, HIGH);

        digitalWrite(LED_BUILTIN, HIGH);
        delay(600);
        digitalWrite(LED_BUILTIN, LOW);
        delay(600);
        break;
      case HIGH_FREQ_ONLY:
        digitalWrite(MOD_PIN, HIGH);
        digitalWrite(CONTROL_PIN, LOW);

        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
        break;
      default:
        digitalWrite(MOD_PIN, LOW);
        digitalWrite(CONTROL_PIN, LOW);
        digitalWrite(LED_BUILTIN, LOW);
        break;
    }
  }

#if defined(INCLUDE_DATA)
  if (state == INIT) {
    state = START;

    while (state != END) {
      digitalWrite(LED_BUILTIN, HIGH);
    }
    Timer1.stop();
    digitalWrite(LED_BUILTIN, LOW);
  }
#else
  if (state == INITIATING && PORT.available() >= 2) {
    PORT.readBytes((char*)&bit_interval_us, 2);
    Timer1.setPeriod(bit_interval_us);

    state = READING;
  }

  while (state == READING && PORT.available()) {
    current_byte = PORT.read();
    bit_idx = 0;
    state = WRITING;
    digitalWrite(LED_BUILTIN, HIGH);
  }
#endif
}
