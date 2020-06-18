#pragma once

// BUMPER PINS
#define BUMPER_SWITCH_FRONT_RH 46 // Define Pin 47 on the MEGA to detect the microswitch
#define BUMPER_SWITCH_FRONT_LH 47 // Define Pin 46 on the MEGA to detect the microswitch

// UART
#define PIN_RX 10
#define PIN_TX 11

// VOLT/AMP/RAIN PINS
#define RAIN_PIN A0
#define AMP_PIN A1
#define VOLT_PIN A2

// PERIMETER PINS
#define PIN_PERIMETER_LEFT A3  // perimeter pin normally A5
#define PIN_PERIMETER_RIGHT A6 // leave open

// I2C
#define PIN_SDA A4
#define PIN_SCL A5

// ULTRASONIC PINS // FROM BEHIND THE MOWER
#define PIN_SONAR_LEFT_TRIGGER 4
#define PIN_SONAR_LEFT_ECHO 5
#define PIN_SONAR_MIDDLE_TRIGGER 6
#define PIN_SONAR_MIDDLE_ECHO 7
#define PIN_SONAR_RIGHT_TRIGGER 8
#define PIN_SONAR_RIGHT_ECHO 9

// LED PINS
#define PIN_LED LED_BUILTIN

// PERIMETER-SETTINGS
#define SIGCODE_1
