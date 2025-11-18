#include <EEPROM.h>
#include "data.h"

#define BUFFER_SIZE 10
#define OUTPUT_PORT 12

uint8_t buffer[BUFFER_SIZE];
uint8_t buffer_len = 0;
uint8_t rate = 100;  // fallback value
uint16_t data_idx = 0;
uint32_t next_time = 0;

extern const uint8_t rand_data[];

void clearBuffer();
uint8_t isValidInput(const uint8_t *buf, uint8_t len);

void setup() {
  Serial.begin(115200);
  pinMode(OUTPUT_PORT, OUTPUT);

  EEPROM.get(0, rate);
  if (rate <= 0) rate = 100;

  Serial.print("RATE = ");
  Serial.println(rate);
}

void loop() {

  if (Serial.available()) {
    uint8_t c = Serial.read();

    // Backspace
    if (c == '\b' || c == 127) {
      if (buffer_len > 0) {
        buffer_len--;
        buffer[buffer_len] = 0;
        Serial.write("\b \b");
      }
      return;
    }

    // CRLF = end of line
    if (c == '\r') {
      // Wait non-blocking for LF
      if (!Serial.available()) return;
      uint8_t next = Serial.read();
      if (next == '\n') {
        if (buffer_len == 0 || !isValidInput(buffer, buffer_len)) {
          Serial.println("FAILED");
          clearBuffer();
          return;
        }

        Serial.print("RATE = ");
        Serial.println((char *)buffer);

        rate = atoi((char *)buffer);
        EEPROM.put(0, rate);

        clearBuffer();
        return;
      }
    }

    // Normal chars
    if (buffer_len < BUFFER_SIZE - 1) {
      buffer[buffer_len++] = c;
      buffer[buffer_len] = 0;
      Serial.write(c);
    }

    return;
  }

  // ---------- CONTINUOUS OUTPUT ----------
  uint32_t now = micros();
  if (rate > 0 && now >= next_time) {
    digitalWrite(OUTPUT_PORT, rand_data[data_idx] ? HIGH : LOW);
    data_idx++;
    if (data_idx >= DATA_LEN) {
      data_idx = 0;
    }
    next_time = now + rate;
  }
}

void clearBuffer() {
  memset(buffer, 0, BUFFER_SIZE);
  buffer_len = 0;
}

uint8_t isValidInput(const uint8_t *buf, uint8_t len) {
  for (uint8_t i = 0; i < len; i++) {
    if (buf[i] < '0' || buf[i] > '9') return 0;
  }
  return 1;
}