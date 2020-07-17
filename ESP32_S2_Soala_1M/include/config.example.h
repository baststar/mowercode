#pragma once

#include <driver/adc.h>

#define USE_NODEMCU 1
#define USE_COMPASS 1

#define USE_WLAN 1
#define WIFI_SSID ""
#define WIFI_PASS ""
#define WLAN_STATIC_IP "192.168.0.230"
#define WLAN_SUBNETMASK "255.255.255.0"
#define WLAN_GATEWAY "192.168.0.1"

#define USE_MQTT 1
#define MQTT_HOST "192.168.0.20"
#define MQTT_HOSTNAME "pi"
#define MQTT_DOMAIN ""
#define MQTT_CLIENT_ID "Mower"
#define MQTT_PORT 8883
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""
#define CERT_SHA1_FINGERPRINT ""

#define IPIFYDOMAIN "https://api.ipify.org/?format=json"

#define USE_BLYNK 0
#define BLYNK_APP_TOKEN ""

#define USE_NODE_RED 0
#define NODE_RED_API_HOST ""
#define NODE_RED_API_USERNAME ""
#define NODE_RED_API_PASSWORD ""


#define MQTT_CA_CERT ""

#define MQTT_CLIENT_CERT ""

#define MQTT_CLIENT_KEY ""


// PERIMETER IS CLOCKWISE (1) OR COUNTERCLOCKWISE (0) FROM GARAGE
#define PERIMETER_IS_CLOCKWISE_FROM_GARAGE 1
// MAX MAGNITUDE TO MEASURE LEFT OR RIGHT FROM CABLE TO FOLLOW THE PERIMETER
// The magnitude 10cm left and 10cm right from wire - view magnitude in testmenu->wire
// Adjust on the potentiometer on the sensor and here
#define MAX_TRACKING_WIRE_MAGNITUDE_INSIDE 1800
#define MAX_TRACKING_WIRE_MAGNITUDE_OUTSIDE 2000
#define MAX_SAME_SIDE_TRACKING_WIRE_TIME 8000 // time until mower makes strong turns while following wire and dont passes the wire from left or right
#define P_VALUE 25                            // will be devided by 100 in code -> 14 = 0.14

// TIMES in milliseconds
#define EXIT_GARAGE_BACKWARDS_TIME 9000
#define RANDOM_ROTATE_TIME_MIN 800
#define RANDOM_ROTATE_TIME_MAX 1300
#define ROTATE_FROM_EXIT_GARAGE_TO_WIRE_TIME 1200
#define ROTATE_FROM_WIRE_TO_GARDEN_TIME 1500
#define FIND_WIRE_SEARCH_TIME_MAX 20000
#define FOLLOW_WIRE_ZONE_1_TIME 4000
#define FOLLOW_WIRE_ZONE_2_TIME 15000
#define WIRE_INTO_TO_GARDEN_TIME 2000

// MAX MOTOR SPEEDS
#define PWM_MAXSPEED_RIGHT 242
#define PWM_MAXSPEED_LEFT 255
#define PWM_SLOWSPEED 160
#define PWM_BLADESPEED 255

// MOWTIMES
#define QUICK_MOW_MOWTIME 2 // minutes
#define ZONE_1_MOWTIME 2    // minutes
#define ZONE_2_MOWTIME 2    // minutes

// Alarms 24h
#define ALARM_HOUR_1 11
#define ALARM_MINUTE_1 0
#define ALARM_ACTIVE_1 1
#define ALARM_SEQUENCE_1 11 // Zone 1

#define ALARM_HOUR_2 12
#define ALARM_MINUTE_2 0
#define ALARM_ACTIVE_2 0
#define ALARM_SEQUENCE_2 12 // Zone 2

#define ALARM_HOUR_3 7
#define ALARM_MINUTE_3 0
#define ALARM_ACTIVE_3 0
#define ALARM_SEQUENCE_3 17 // Quick Mow

// UART
#define PIN_RX GPIO_NUM_39 // 39
#define PIN_TX GPIO_NUM_38 // 38

// DISPLAY
#define PIN_SDA_DISPLAY GPIO_NUM_21 // 21
#define PIN_SCL_DISPLAY GPIO_NUM_26 // 26

// KEYBOARD PINS
#define PIN_PLUS_KEY GPIO_NUM_10  // 10 // digital
#define PIN_START_KEY GPIO_NUM_11 // 11 // digital
#define PIN_STOP_KEY GPIO_NUM_12  // 12 // digital
#define PIN_MINUS_KEY GPIO_NUM_13 // 13 // digital

// MOTOR A PINS
#define PIN_MOTOR_1_ENA GPIO_NUM_1   // 1  // EN Pins need a digital pin with PWM // PWM
#define PIN_MOTOR_1_IN_1 GPIO_NUM_33 // 33 // digital
#define PIN_MOTOR_1_IN_2 GPIO_NUM_34 // 34  // IN Pins dont need digital PWM // digital
// MOTOR B PINS
#define PIN_MOTOR_2_ENB GPIO_NUM_2   // 2 // // EN Pins need a digital pin with PWM // PWM
#define PIN_MOTOR_2_IN_1 GPIO_NUM_35 // 35 // digital
#define PIN_MOTOR_2_IN_2 GPIO_NUM_36 // 36 // IN Pins dont need digital PWM // digital

// MOTOR BLADES PINS
#define PIN_BLADES_RPWM GPIO_NUM_3 // 3 // PWM
#define PIN_BLADES_L_EN GPIO_NUM_4 // 4 // digital
#define PIN_BLADES_R_EN GPIO_NUM_5 // 5 // digital

// BUMPER PINS
// #define BUMPER_SWITCH_FRONT_RH 46 // Define Pin 47 on the MEGA to detect the microswitch // digital
// #define BUMPER_SWITCH_FRONT_LH 47 // Define Pin 46 on the MEGA to detect the microswitch // digital

// VOLT PIN
#define PIN_VOLT_SENSOR ADC1_CHANNEL_5 // 6

// RELAY PINS
#define PIN_RELAY_MOTORS GPIO_NUM_7 // 7

// AMP PIN
#define PIN_AMP_SENSOR ADC1_CHANNEL_7 // 8

// RAIN PIN
#define PIN_RAIN_SENSOR ADC1_CHANNEL_8 // 9

// LED PINS
#define PIN_LED ADC2_CHANNEL_7 // 18
