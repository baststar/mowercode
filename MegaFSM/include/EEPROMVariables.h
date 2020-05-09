#pragma once

#include <Arduino.h>

// 2 byte unsigned int (uint16_t) max 65535
// todo: check for max values of 65535 in settings-menues

// PERIMETER
extern uint16_t eeprom_perimeter_is_clockwise_from_garage;
#define EEPROM_INDEX_PERIMETER_IS_CLOCKWISE_FROM_GARAGE 0
extern uint16_t eeprom_max_tracking_wire_magnitude_inside;
#define EEPROM_INDEX_MAX_TRACKING_WIRE_MAGNITUDE_INSIDE 2
extern uint16_t eeprom_max_tracking_wire_magnitude_outside;
#define EEPROM_INDEX_MAX_TRACKING_WIRE_MAGNITUDE_OUTSIDE 4
extern uint16_t eeprom_max_same_side_tracking_wire_time;
#define EEPROM_INDEX_MAX_SAME_SIDE_TRACKING_WIRE_TIME 6

// TIMES in milliseconds
extern uint16_t eeprom_exit_garage_backwards_time;
#define EEPROM_INDEX_EXIT_GARAGE_BACKWARDS_TIME 8
extern uint16_t eeprom_random_rotate_time_min;
#define EEPROM_INDEX_RANDOM_ROTATE_TIME_MIN 10
extern uint16_t eeprom_random_rotate_time_max;
#define EEPROM_INDEX_RANDOM_ROTATE_TIME_MAX 12
extern uint16_t eeprom_rotate_from_exit_garage_to_wire_time;
#define EEPROM_INDEX_ROTATE_FROM_EXIT_GARAGE_TO_WIRE_TIME 14
extern uint16_t eeprom_rotate_from_wire_to_garden_time;
#define EEPROM_INDEX_ROTATE_FROM_WIRE_TO_GARDEN_TIME 16
extern uint16_t eeprom_find_wire_search_time_max;
#define EEPROM_INDEX_FIND_WIRE_SEARCH_TIME_MAX 18
extern uint16_t eeprom_follow_wire_zone_1_time;
#define EEPROM_INDEX_FOLLOW_WIRE_ZONE_1_TIME 20
extern uint16_t eeprom_follow_wire_zone_2_time;
#define EEPROM_INDEX_FOLLOW_WIRE_ZONE_2_TIME 22
extern uint16_t eeprom_wire_into_garden_time;
#define EEPROM_INDEX_WIRE_INTO_TO_GARDEN_TIME 24

// MOTOR SPEEDS
extern uint16_t eeprom_pwm_maxspeed_right;
#define EEPROM_INDEX_PWM_MAXSPEED_RIGHT 26
extern uint16_t eeprom_pwm_maxspeed_left;
#define EEPROM_INDEX_PWM_MAXSPEED_LEFT 28
extern uint16_t eeprom_pwm_slowspeed;
#define EEPROM_INDEX_PWM_SLOWSPEED 30
extern uint16_t eeprom_pwm_bladespeed;
#define EEPROM_INDEX_PWM_BLADESPEED 32

// MOWTIMES
#define EEPROM_INDEX_QUICK_MOW_MOWTIME 34
extern uint16_t eeprom_quick_mow_mowtime;
#define EEPROM_INDEX_EXIT_GARAGE_MOWTIME 36
extern uint16_t eeprom_exit_garage_mowtime;
#define EEPROM_INDEX_ALARM_MOWTIME_1 38
extern uint16_t eeprom_alarm_mowtime_1;
#define EEPROM_INDEX_ALARM_MOWTIME_2 40
extern uint16_t eeprom_alarm_mowtime_2;
#define EEPROM_INDEX_ALARM_MOWTIME_3 42
extern uint16_t eeprom_alarm_mowtime_3;

// ALARMS
#define EEPROM_INDEX_ALARM_HOUR_1 44
extern uint16_t eeprom_alarm_hour_1;
#define EEPROM_INDEX_ALARM_MINUTE_1 46
extern uint16_t eeprom_alarm_minute_1;
#define EEPROM_INDEX_ALARM_ACTIVE_1 48
extern uint16_t eeprom_alarm_active_1;

#define EEPROM_INDEX_ALARM_HOUR_2 50
extern uint16_t eeprom_alarm_hour_2;
#define EEPROM_INDEX_ALARM_MINUTE_2 52
extern uint16_t eeprom_alarm_minute_2;
#define EEPROM_INDEX_ALARM_ACTIVE_2 54
extern uint16_t eeprom_alarm_active_2;

#define EEPROM_INDEX_ALARM_HOUR_3 56
extern uint16_t eeprom_alarm_hour_3;
#define EEPROM_INDEX_ALARM_MINUTE_3 58
extern uint16_t eeprom_alarm_minute_3;
#define EEPROM_INDEX_ALARM_ACTIVE_3 60
extern uint16_t eeprom_alarm_active_3;


void PrintEEPROM();
void ResetEEPROM();
void SetupVariablesFromEEPROM();
void SaveIntToEEPROM(int eepromindex, uint16_t value);
uint16_t ReadIntFromEEPROM(int eepromindex);
