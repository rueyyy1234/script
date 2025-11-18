#include <EEPROM.h>
#include "data.h"

#define BUFFER_SIZE 10
#define CONTROL_PORT 12 // Port to control MOSFET gate driver
#define MOD_PORT 7 // Port to EXT MOD

#define RATE_MIN 1
#define RATE_MAX 500000

uint8_t buffer[BUFFER_SIZE];
uint8_t buffer_len = 0;

uint32_t bit_interval_us = 1000000UL;
uint32_t data_rate_hz = 1;

uint16_t data_idx = 0;
uint32_t next_time_us = 0;

extern const uint8_t rand_data[];

void clearBuffer();
uint8_t isDigits(const uint8_t *buf, uint8_t len);

void setup() {
  Serial.begin(9600);
  Serial.flush();
  pinMode(CONTROL_PORT, OUTPUT);
  pinMode(MOD_PORT, OUTPUT);

  // Load saved BPS
  EEPROM.get(0, data_rate_hz);
  if (data_rate_hz < RATE_MIN || data_rate_hz > RATE_MAX) {
    data_rate_hz = 1;
  }

  bit_interval_us = 1000000UL / data_rate_hz;

  Serial.print("\nRATE = ");
  Serial.print(data_rate_hz);
  Serial.println(" Hz");
}

void loop() {

  // ---------- READ SERIAL INPUT ----------
  if (Serial.available()) {
    uint8_t c = Serial.read();
    // Validate input
    if ((c >= '0' && c <= '9') || c == '?') {
      if (buffer_len < BUFFER_SIZE - 1) {
        buffer[buffer_len++] = c;
        buffer[buffer_len] = 0;
        Serial.write(c);
      }
    }

    // BACKSPACE
    if (c == '\b' || c == 127) {
      if (buffer_len > 0) {
        buffer_len--;
        buffer[buffer_len] = 0;
        Serial.write("\b \b");
      }
      return;
    }

    // END OF LINE
    if (c == '\r' || c == '\n') {

      if (c == '\r' && Serial.peek() == '\n') Serial.read();

      // Empty line
      if (buffer_len == 0) {
        clearBuffer();
        return;
      }

      // QUERY CURRENT RATE
      if (buffer_len == 1 && buffer[0] == '?') {
        Serial.print("\r\nRATE = ");
        Serial.print(data_rate_hz);
        Serial.println(" Hz");
        clearBuffer();
        return;
      }

      // Validate input
      if (!isDigits(buffer, buffer_len)) {
        Serial.println("FAILED");
        clearBuffer();
        return;
      }

      // Convert char to int
      uint32_t input_rate = atol((char *)buffer);

      // Validate range
      if (input_rate < RATE_MIN || input_rate > RATE_MAX) {
        Serial.println("OUT OF RANGE");
        clearBuffer();
        return;
      }
      // Save to EEPROM
      data_rate_hz = input_rate;
      EEPROM.put(0, data_rate_hz);

      // Hz → us/bit (bit interval)
      bit_interval_us = 1000000UL / data_rate_hz;

      Serial.print("\r\nSet RATE = ");
      Serial.print(data_rate_hz);
      Serial.println(" bps");

      clearBuffer();
      return;
    }

    return;
  }

  // ---------- OUTPUT ----------
  uint32_t now_us = micros();
  if (now_us >= next_time_us) {
    digitalWrite(CONTROL_PORT, rand_data[data_idx] ? HIGH : LOW);
    digitalWrite(MOD_PORT, rand_data[data_idx] ? LOW : HIGH);

    data_idx++;
    if (data_idx >= DATA_LEN) data_idx = 0;

    next_time_us = now_us + bit_interval_us;
  }
}

void clearBuffer() {
  memset(buffer, 0, BUFFER_SIZE);
  buffer_len = 0;
}

uint8_t isDigits(const uint8_t *buf, uint8_t len) {
  for (uint8_t i = 0; i < len; i++) {
    if (buf[i] < '0' || buf[i] > '9') return 0;
  }
  return 1;
}
