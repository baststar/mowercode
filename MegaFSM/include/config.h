#pragma once

#define LCD_ADDR 0x27 // uint8_t

// ARDUINO MEGA PINS
#define PIN_REALTIME_CLOCK_RESET 29 // uint8_t
#define PIN_REALTIME_CLOCK_DATA 30  // uint8_t
#define PIN_REALTIME_CLOCK_SCL 31   // uint8_t

// KEYBOARD PINS
#define PIN_START_KEY 50
#define PIN_PLUS_KEY 51
#define PIN_MINUS_KEY 52
#define PIN_STOP_KEY 53

// MOTOR A PINS
#define PIN_ENA 7  // EN Pins need a digital pin with PWM
#define PIN_IN_1 6 // IN Pins dont need digital PWM
#define PIN_IN_2 5
// MOTOR B PINS
#define PIN_ENB 2  // EN Pins need a digital pin with PWM
#define PIN_IN_3 4 // IN Pins dont need digital PWM
#define PIN_IN_4 3
// MOTOR BLADES PINS
#define PIN_RPWM 8
#define PIN_L_EN 9
#define PIN_R_EN 10

// BUMPER PINS
#define BUMPER_SWITCH_FRONT_RH 46 // Define Pin 47 on the MEGA to detect the microswitch
#define BUMPER_SWITCH_FRONT_LH 47 // Define Pin 46 on the MEGA to detect the microswitch

// RELAY PINS
#define PIN_RELAY_MOTORS 24
