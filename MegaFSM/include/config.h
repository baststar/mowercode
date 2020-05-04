#pragma once

// PERIMETER IS CLOCKWISE (true) OR COUNTERCLOCKWISE (false) FROM GARAGE
#define PERIMETER_IS_CLOCKWISE_FROM_GARAGE true
// MAX MAGNITUDE TO MEASURE LEFT OR RIGHT FROM CABLE TO FOLLOW THE PERIMETER
// The magnitude 10cm left and 10cm right from wire - view magnitude in testmenu->wire
// Adjust on the potentiometer on the sensor and here
#define MAX_TRACKING_WIRE_MAGNITUDE_INSIDE 1100
#define MAX_TRACKING_WIRE_MAGNITUDE_OUTSIDE 700
#define MAX_SAME_SITE_TRACKING_WIRE_TIME 2000 // time until mower makes strong turns while following wire and dont passes the wire from left or right

// TIMES in milliseconds
#define EXIT_GARAGE_BACKWARDS_TIME 10000
#define RANDOM_ROTATE_TIME_MIN 1400
#define RANDOM_ROTATE_TIME_MAX 1800
#define ROTATE_FROM_EXIT_GARAGE_TO_WIRE_TIME 1000
#define ROTATE_FROM_WIRE_TO_GARDEN_TIME 1000
#define FIND_WIRE_SEARCH_TIME_MAX 20000
#define FOLLOW_WIRE_ZONE_1_TIME 4000
#define FOLLOW_WIRE_ZONE_2_TIME 15000
#define WIRE_INTO_TO_GARDEN_TIME 4000

// MAX MOTOR SPEEDS
#define PWM_MAXSPEED_RIGHT 242
#define PWM_MAXSPEED_LEFT 255
#define PWM_SLOWSPEED_SUBSTRACTION 100
#define PWM_BLADESPEED 255

// LCD ADDRESS
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

// PERIMETER PINS
#define PIN_PERIMETER_LEFT A5  // perimeter pin normally A5
#define PIN_PERIMETER_RIGHT A4 // leave open

// LED PINS
#define PIN_LED LED_BUILTIN
