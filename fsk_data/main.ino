#define CONTROL_PORT 12
#define MOD_PORT 7

uint32_t bit_interval_us = 67;
uint32_t offset = 6;
uint32_t next_time_us = 0;
uint8_t c = 0;
uint8_t bit;
void setup() {
  Serial.begin(460800);
  pinMode(CONTROL_PORT, OUTPUT);
  pinMode(MOD_PORT, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  while (Serial.available()) {
    uint32_t now_us = micros();

    c = Serial.read();
    // send 8 bits, LSB first
    for (uint8_t i = 0; i < 8; i++) {
      bit = (c >> i) & 0x01;
      while (now_us < next_time_us) {
        now_us = micros();
      }
      if (bit) {
        PORTB |= (1 << PB4);
        PORTD &= ~(1 << PD7);
      } else {
        PORTB &= ~(1 << PB4); 
        PORTD |= (1 << PD7);
      }

      next_time_us = now_us + (bit_interval_us - offset);
    }
  }
}
