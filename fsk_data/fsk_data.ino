#include <avr/io.h>
#include <avr/interrupt.h>

#define CONTROL_PIN PB4
#define MOD_PIN     PD7

uint8_t currentByte = 0;
uint8_t bitIndex = 0;
uint8_t readComplete = 0;

const uint16_t bit_interval_us = 30;

void initTimer1(uint16_t us) {
    // Timer/Counter control registers - reset
    TCCR1A = 0;
    TCCR1B = 0; 

    // Timer/Counter control registers
    TCCR1B |= (1 << WGM12); // Clear timer on compare mode, update OCR1A immediately
    TCCR1B |= (1 << CS11);  // CLK_IO/8 prescale

    // OCRnx = f_clk_io/(f_OCnx*2*N) - 1, while N = 8
    OCR1A = (us * 2) - 0; // Call ISR when OCR1A value reached
    TIMSK1 |= (1 << OCIE1A); // Timer/Counter1, Output Compare A Match Interrupt Enable
}

void setup() {
    Serial.begin(460800);
    DDRB |= (1 << CONTROL_PIN);
    DDRD |= (1 << MOD_PIN);
    initTimer1(bit_interval_us);

    // Enable ISR
    sei();
}

// Timer/Counter1 Compare Match A
ISR(TIMER1_COMPA_vect) {

    if (!readComplete) return;

    uint8_t bit = (currentByte >> (7 - bitIndex)) & 0x01;

    if (bit) {
        PORTB |=  (1 << CONTROL_PIN);
        PORTD &= ~(1 << MOD_PIN);
    } else {
        PORTB &= ~(1 << CONTROL_PIN);
        PORTD |=  (1 << MOD_PIN);
    }

    bitIndex++;

    if (bitIndex >= 8) {
        // 8 bits output complete - proceed to read
        readComplete = 0;
    }
}

void loop() {

    if (Serial.available() && !readComplete) {
        currentByte = Serial.read();
        // Byte received - proceed to output
        bitIndex = 0;
        readComplete = 1;
    }
}
