#pragma once

#include <Arduino.h>

// PERIMETER
extern uint16_t eeprom_perimeter_is_clockwise_from_garage;
#define EEPROM_INDEX_PERIMETER_IS_CLOCKWISE_FROM_GARAGE 0
extern uint16_t eeprom_max_tracking_wire_magnitude_inside;
#define EEPROM_INDEX_MAX_TRACKING_WIRE_MAGNITUDE_INSIDE 10
extern uint16_t eeprom_max_tracking_wire_magnitude_outside;
#define EEPROM_INDEX_MAX_TRACKING_WIRE_MAGNITUDE_OUTSIDE 20
extern uint16_t eeprom_max_same_side_tracking_wire_time;
#define EEPROM_INDEX_MAX_SAME_SIDE_TRACKING_WIRE_TIME 30

// TIMES in milliseconds
extern uint16_t eeprom_exit_garage_backwards_time;
#define EEPROM_INDEX_EXIT_GARAGE_BACKWARDS_TIME 40
extern uint16_t eeprom_random_rotate_time_min;
#define EEPROM_INDEX_RANDOM_ROTATE_TIME_MIN 50
extern uint16_t eeprom_random_rotate_time_max;
#define EEPROM_INDEX_RANDOM_ROTATE_TIME_MAX 60
extern uint16_t eeprom_rotate_from_exit_garage_to_wire_time;
#define EEPROM_INDEX_ROTATE_FROM_EXIT_GARAGE_TO_WIRE_TIME 70
extern uint16_t eeprom_rotate_from_wire_to_garden_time;
#define EEPROM_INDEX_ROTATE_FROM_WIRE_TO_GARDEN_TIME 80
extern uint16_t eeprom_find_wire_search_time_max;
#define EEPROM_INDEX_FIND_WIRE_SEARCH_TIME_MAX 90
extern uint16_t eeprom_follow_wire_zone_1_time;
#define EEPROM_INDEX_FOLLOW_WIRE_ZONE_1_TIME 100
extern uint16_t eeprom_follow_wire_zone_2_time;
#define EEPROM_INDEX_FOLLOW_WIRE_ZONE_2_TIME 110
extern uint16_t eeprom_wire_into_garden_time;
#define EEPROM_INDEX_WIRE_INTO_TO_GARDEN_TIME 120

// MOTOR SPEEDS
extern uint16_t eeprom_pwm_maxspeed_right;
#define EEPROM_INDEX_PWM_MAXSPEED_RIGHT 130
extern uint16_t eeprom_pwm_maxspeed_left;
#define EEPROM_INDEX_PWM_MAXSPEED_LEFT 140
extern uint16_t eeprom_pwm_slowspeed;
#define EEPROM_INDEX_PWM_SLOWSPEED 150
extern uint16_t eeprom_pwm_bladespeed;
#define EEPROM_INDEX_PWM_BLADESPEED 160

void PrintEEPROM();
void ResetEEPROM();
void SetupVariablesFromEEPROM();
void SaveIntToEEPROM(int eepromindex, uint16_t value);
uint16_t ReadIntFromEEPROM(int eepromindex);
