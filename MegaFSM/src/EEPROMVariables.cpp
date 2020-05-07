#include <EEPROMVariables.h>
#include <EEPROMex.h>
#include <LCD.h>
#include <config.h>

// Arduino ATMega1280 and ATMega2560 : 4kb EEPROM storage.

// PERIMETER
uint16_t eeprom_perimeter_is_clockwise_from_garage = PERIMETER_IS_CLOCKWISE_FROM_GARAGE;
uint16_t eeprom_max_tracking_wire_magnitude_inside = MAX_TRACKING_WIRE_MAGNITUDE_INSIDE;
uint16_t eeprom_max_tracking_wire_magnitude_outside = MAX_TRACKING_WIRE_MAGNITUDE_OUTSIDE;
uint16_t eeprom_max_same_side_tracking_wire_time = MAX_SAME_SIDE_TRACKING_WIRE_TIME;

// TIMES
uint16_t eeprom_exit_garage_backwards_time = EXIT_GARAGE_BACKWARDS_TIME;
uint16_t eeprom_random_rotate_time_min = RANDOM_ROTATE_TIME_MIN;
uint16_t eeprom_random_rotate_time_max = RANDOM_ROTATE_TIME_MAX;
uint16_t eeprom_rotate_from_exit_garage_to_wire_time = ROTATE_FROM_EXIT_GARAGE_TO_WIRE_TIME;
uint16_t eeprom_rotate_from_wire_to_garden_time = ROTATE_FROM_WIRE_TO_GARDEN_TIME;
uint16_t eeprom_find_wire_search_time_max = FIND_WIRE_SEARCH_TIME_MAX;
uint16_t eeprom_follow_wire_zone_1_time = FOLLOW_WIRE_ZONE_1_TIME;
uint16_t eeprom_follow_wire_zone_2_time = FOLLOW_WIRE_ZONE_2_TIME;
uint16_t eeprom_wire_into_garden_time = WIRE_INTO_TO_GARDEN_TIME;

// MOTOR SPEEDS
uint16_t eeprom_pwm_maxspeed_right = PWM_MAXSPEED_RIGHT;
uint16_t eeprom_pwm_maxspeed_left = PWM_MAXSPEED_LEFT;
uint16_t eeprom_pwm_slowspeed = PWM_SLOWSPEED;
uint16_t eeprom_pwm_bladespeed = PWM_BLADESPEED;

void PrintEEPROM() {

    if (EEPROM.isReady()) {

        // PERIMETER
        Serial.println("PERIMETER_IS_CLOCKWISE_FROM_GARAGE " + String(EEPROM.readInt(EEPROM_INDEX_PERIMETER_IS_CLOCKWISE_FROM_GARAGE)));
        Serial.println("MAX_TRACKING_WIRE_MAGNITUDE_INSIDE " + String(EEPROM.readInt(EEPROM_INDEX_MAX_TRACKING_WIRE_MAGNITUDE_INSIDE)));
        Serial.println("MAX_TRACKING_WIRE_MAGNITUDE_OUTSIDE " + String(EEPROM.readInt(EEPROM_INDEX_MAX_TRACKING_WIRE_MAGNITUDE_OUTSIDE)));
        Serial.println("MAX_SAME_SIDE_TRACKING_WIRE_TIME " + String(EEPROM.readInt(EEPROM_INDEX_MAX_SAME_SIDE_TRACKING_WIRE_TIME)));

        // TIMES
        Serial.println("EXIT_GARAGE_BACKWARDS_TIME " + String(EEPROM.readInt(EEPROM_INDEX_EXIT_GARAGE_BACKWARDS_TIME)));
        Serial.println("RANDOM_ROTATE_TIME_MIN " + String(EEPROM.readInt(EEPROM_INDEX_RANDOM_ROTATE_TIME_MIN)));
        Serial.println("RANDOM_ROTATE_TIME_MAX " + String(EEPROM.readInt(EEPROM_INDEX_RANDOM_ROTATE_TIME_MAX)));
        Serial.println("ROTATE_FROM_EXIT_GARAGE_TO_WIRE_TIME " + String(EEPROM.readInt(EEPROM_INDEX_ROTATE_FROM_EXIT_GARAGE_TO_WIRE_TIME)));
        Serial.println("ROTATE_FROM_WIRE_TO_GARDEN_TIME " + String(EEPROM.readInt(EEPROM_INDEX_ROTATE_FROM_WIRE_TO_GARDEN_TIME)));
        Serial.println("FIND_WIRE_SEARCH_TIME_MAX " + String(EEPROM.readInt(EEPROM_INDEX_FIND_WIRE_SEARCH_TIME_MAX)));
        Serial.println("FOLLOW_WIRE_ZONE_1_TIME " + String(EEPROM.readInt(EEPROM_INDEX_FOLLOW_WIRE_ZONE_1_TIME)));
        Serial.println("FOLLOW_WIRE_ZONE_2_TIME " + String(EEPROM.readInt(EEPROM_INDEX_FOLLOW_WIRE_ZONE_2_TIME)));
        Serial.println("WIRE_INTO_TO_GARDEN_TIME " + String(EEPROM.readInt(EEPROM_INDEX_WIRE_INTO_TO_GARDEN_TIME)));

        // MAX MOTOR SPE
        Serial.println("PWM_MAXSPEED_RIGHT " + String(EEPROM.readInt(EEPROM_INDEX_PWM_MAXSPEED_RIGHT)));
        Serial.println("PWM_MAXSPEED_LEFT " + String(EEPROM.readInt(EEPROM_INDEX_PWM_MAXSPEED_LEFT)));
        Serial.println("PWM_SLOWSPEED " + String(EEPROM.readInt(EEPROM_INDEX_PWM_SLOWSPEED)));
        Serial.println("PWM_BLADESPEED " + String(EEPROM.readInt(EEPROM_INDEX_PWM_BLADESPEED)));

    } else {
        ShowError("EEPROM not ready");
    }
}

void SetupVariablesFromEEPROM() {

    if (EEPROM.isReady()) {

        // PERIMETER
        eeprom_perimeter_is_clockwise_from_garage = EEPROM.readInt(EEPROM_INDEX_PERIMETER_IS_CLOCKWISE_FROM_GARAGE);
        eeprom_max_tracking_wire_magnitude_inside = EEPROM.readInt(EEPROM_INDEX_MAX_TRACKING_WIRE_MAGNITUDE_INSIDE);
        eeprom_max_tracking_wire_magnitude_outside = EEPROM.readInt(EEPROM_INDEX_MAX_TRACKING_WIRE_MAGNITUDE_OUTSIDE);
        eeprom_max_same_side_tracking_wire_time = EEPROM.readInt(EEPROM_INDEX_MAX_SAME_SIDE_TRACKING_WIRE_TIME);

        // TIMES
        eeprom_exit_garage_backwards_time = EEPROM.readInt(EEPROM_INDEX_EXIT_GARAGE_BACKWARDS_TIME);
        eeprom_random_rotate_time_min = EEPROM.readInt(EEPROM_INDEX_RANDOM_ROTATE_TIME_MIN);
        eeprom_random_rotate_time_max = EEPROM.readInt(EEPROM_INDEX_RANDOM_ROTATE_TIME_MAX);
        eeprom_rotate_from_exit_garage_to_wire_time = EEPROM.readInt(EEPROM_INDEX_ROTATE_FROM_EXIT_GARAGE_TO_WIRE_TIME);
        eeprom_rotate_from_wire_to_garden_time = EEPROM.readInt(EEPROM_INDEX_ROTATE_FROM_WIRE_TO_GARDEN_TIME);
        eeprom_find_wire_search_time_max = EEPROM.readInt(EEPROM_INDEX_FIND_WIRE_SEARCH_TIME_MAX);
        eeprom_follow_wire_zone_1_time = EEPROM.readInt(EEPROM_INDEX_FOLLOW_WIRE_ZONE_1_TIME);
        eeprom_follow_wire_zone_2_time = EEPROM.readInt(EEPROM_INDEX_FOLLOW_WIRE_ZONE_2_TIME);
        eeprom_wire_into_garden_time = EEPROM.readInt(EEPROM_INDEX_WIRE_INTO_TO_GARDEN_TIME);

        // MAX MOTOR SPEEDS
        eeprom_pwm_maxspeed_right = EEPROM.readInt(EEPROM_INDEX_PWM_MAXSPEED_RIGHT);
        eeprom_pwm_maxspeed_left = EEPROM.readInt(EEPROM_INDEX_PWM_MAXSPEED_LEFT);
        eeprom_pwm_slowspeed = EEPROM.readInt(EEPROM_INDEX_PWM_SLOWSPEED);
        eeprom_pwm_bladespeed = EEPROM.readInt(EEPROM_INDEX_PWM_BLADESPEED);

    } else {
        ShowError("EEPROM not ready");
    }
}

void ResetEEPROM() {

    if (EEPROM.isReady()) {

        // PERIMETER
        EEPROM.writeInt(EEPROM_INDEX_PERIMETER_IS_CLOCKWISE_FROM_GARAGE, PERIMETER_IS_CLOCKWISE_FROM_GARAGE);
        EEPROM.writeInt(EEPROM_INDEX_MAX_TRACKING_WIRE_MAGNITUDE_INSIDE, MAX_TRACKING_WIRE_MAGNITUDE_INSIDE);
        EEPROM.writeInt(EEPROM_INDEX_MAX_TRACKING_WIRE_MAGNITUDE_OUTSIDE, MAX_TRACKING_WIRE_MAGNITUDE_OUTSIDE);
        EEPROM.writeInt(EEPROM_INDEX_MAX_SAME_SIDE_TRACKING_WIRE_TIME, MAX_SAME_SIDE_TRACKING_WIRE_TIME);

        // TIMES
        EEPROM.writeInt(EEPROM_INDEX_EXIT_GARAGE_BACKWARDS_TIME, EXIT_GARAGE_BACKWARDS_TIME);
        EEPROM.writeInt(EEPROM_INDEX_RANDOM_ROTATE_TIME_MIN, RANDOM_ROTATE_TIME_MIN);
        EEPROM.writeInt(EEPROM_INDEX_RANDOM_ROTATE_TIME_MAX, RANDOM_ROTATE_TIME_MAX);
        EEPROM.writeInt(EEPROM_INDEX_ROTATE_FROM_EXIT_GARAGE_TO_WIRE_TIME, ROTATE_FROM_EXIT_GARAGE_TO_WIRE_TIME);
        EEPROM.writeInt(EEPROM_INDEX_ROTATE_FROM_WIRE_TO_GARDEN_TIME, ROTATE_FROM_WIRE_TO_GARDEN_TIME);
        EEPROM.writeInt(EEPROM_INDEX_FIND_WIRE_SEARCH_TIME_MAX, FIND_WIRE_SEARCH_TIME_MAX);
        EEPROM.writeInt(EEPROM_INDEX_FOLLOW_WIRE_ZONE_1_TIME, FOLLOW_WIRE_ZONE_1_TIME);
        EEPROM.writeInt(EEPROM_INDEX_FOLLOW_WIRE_ZONE_2_TIME, FOLLOW_WIRE_ZONE_2_TIME);
        EEPROM.writeInt(EEPROM_INDEX_WIRE_INTO_TO_GARDEN_TIME, WIRE_INTO_TO_GARDEN_TIME);

        // MAX MOTOR SPEEDS
        EEPROM.writeInt(EEPROM_INDEX_PWM_MAXSPEED_RIGHT, PWM_MAXSPEED_RIGHT);
        EEPROM.writeInt(EEPROM_INDEX_PWM_MAXSPEED_LEFT, PWM_MAXSPEED_LEFT);
        EEPROM.writeInt(EEPROM_INDEX_PWM_SLOWSPEED, PWM_SLOWSPEED);
        EEPROM.writeInt(EEPROM_INDEX_PWM_BLADESPEED, PWM_BLADESPEED);

        ShowError("EEPROM RESETTET");
    } else {
        ShowError("EEPROM not ready");
    }
}

void SaveIntToEEPROM(int eepromindex, uint16_t value) {
    if (EEPROM.isReady()) {
        EEPROM.writeInt(eepromindex, value);
    } else {
        ShowError("EEPROM not ready");
    }
}

uint16_t ReadIntFromEEPROM(int eepromindex) {
    if (EEPROM.isReady()) {
        return EEPROM.readInt(eepromindex);
    } else {
        ShowError("EEPROM not ready");
    }
}
