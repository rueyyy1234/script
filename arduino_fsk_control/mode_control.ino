#include "handler.h"

uint8_t programMode;


void setup_pins();
uint8_t get_mode();
void setup_mode(uint8_t mode);


void setup() {
  setup_pins();
  programMode = get_mode();
  setup_mode(programMode);
}


void loop() {
  run_handler();
  error_handler();
}


void setup_pins() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(MOD_PIN, OUTPUT);
  pinMode(CONTROL_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);


  pinMode(SWITCH_PIN_1, INPUT_PULLUP);
  pinMode(SWITCH_PIN_2, INPUT_PULLUP);
  pinMode(SWITCH_PIN_3, INPUT_PULLUP);
  pinMode(SWITCH_PIN_4, INPUT_PULLUP);
}


uint8_t get_mode() {
  uint8_t val;
  uint8_t mode = 0;


  val = digitalRead(SWITCH_PIN_4);
  mode |= ((~val) & 0x01);


  val = digitalRead(SWITCH_PIN_3);
  mode |= (((~val) & 0x01) << 1);


  val = digitalRead(SWITCH_PIN_2);
  mode |= (((~val) & 0x01) << 2);


  val = digitalRead(SWITCH_PIN_1);
  mode |= (((~val) & 0x01) << 3);


  return mode;
}

void setup_mode(uint8_t mode) {
  switch (mode) {
    case MODE_0:
      register_handler(mode_0_handler);
      break;
    case MODE_1:
      register_handler(mode_1_handler);
      break;
    case MODE_2:
      register_handler(mode_2_handler);
      break;
    case MODE_3:
      register_handler(mode_3_handler);
      break;
    case MODE_4:
      register_handler(mode_4_handler);
      break;
    case MODE_5:
      register_handler(mode_5_handler);
      break;
    case MODE_6:
      register_handler(mode_6_handler);
      break;
    case MODE_7:
      register_handler(mode_7_handler);
      break;
    case MODE_8:
      register_handler(mode_8_handler);
      break;
    case MODE_9:
      register_handler(mode_9_handler);
      break;
    case MODE_10:
      register_handler(mode_10_handler);
      break;
    case MODE_11:
      register_handler(mode_11_handler);
      break;
    case MODE_12:
      register_handler(mode_12_handler);
      break;
    case MODE_13:
      register_handler(mode_13_handler);
      break;
    case MODE_14:
      register_handler(mode_14_handler);
      break;
    case MODE_15:
      register_handler(mode_15_handler);
      break;
    default:
      error_handler();
      break;
  }
}