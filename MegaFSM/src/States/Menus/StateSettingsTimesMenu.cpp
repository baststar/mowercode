#include <CustomFunctions.h>
#include <EEPROMVariables.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMStates.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/Menus/StateSettingsTimesMenu.h>
#include <config.h>


int settingsTimesMenu_currentMenu = 0;
String settingsTimesMenuNames[] = {"LEAVE GARAGE",  "RAND. ROTATE MIN", "RAND. ROTATE MAX", "ROT. AFTER GARAGE", "ROT. TO GARDEN",
                                   "FIND WIRE MAX", "ZONE 1 DISTANCE",  "ZONE 2 DISTANCE",  "WIRE TO GARDEN"};

void read_settingsTimes_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMTransition = currentFSMTransition;
        TriggerFSM(STATE_SETTINGS_TIMES_MENU, STATE_SETTINGS_MENU, -1);
        return;
    } else if (PlusKey_pressed == 0) {
        delay(250);
        if (settingsTimesMenu_currentMenu == 0) {
            eeprom_exit_garage_backwards_time += 500;
            SaveIntToEEPROM(EEPROM_INDEX_EXIT_GARAGE_BACKWARDS_TIME, eeprom_exit_garage_backwards_time);
        } else if (settingsTimesMenu_currentMenu == 1) {
            eeprom_random_rotate_time_min += 500;
            SaveIntToEEPROM(EEPROM_INDEX_RANDOM_ROTATE_TIME_MIN, eeprom_random_rotate_time_min);
        } else if (settingsTimesMenu_currentMenu == 2) {
            eeprom_random_rotate_time_max += 500;
            SaveIntToEEPROM(EEPROM_INDEX_RANDOM_ROTATE_TIME_MAX, eeprom_random_rotate_time_max);
        } else if (settingsTimesMenu_currentMenu == 3) {
            eeprom_rotate_from_exit_garage_to_wire_time += 500;
            SaveIntToEEPROM(EEPROM_INDEX_ROTATE_FROM_EXIT_GARAGE_TO_WIRE_TIME, eeprom_rotate_from_exit_garage_to_wire_time);
        } else if (settingsTimesMenu_currentMenu == 4) {
            eeprom_rotate_from_wire_to_garden_time += 500;
            SaveIntToEEPROM(EEPROM_INDEX_ROTATE_FROM_WIRE_TO_GARDEN_TIME, eeprom_rotate_from_wire_to_garden_time);
        } else if (settingsTimesMenu_currentMenu == 5) {
            eeprom_find_wire_search_time_max += 500;
            SaveIntToEEPROM(EEPROM_INDEX_FIND_WIRE_SEARCH_TIME_MAX, eeprom_find_wire_search_time_max);
        } else if (settingsTimesMenu_currentMenu == 6) {
            eeprom_follow_wire_zone_1_time += 500;
            SaveIntToEEPROM(EEPROM_INDEX_FOLLOW_WIRE_ZONE_1_TIME, eeprom_follow_wire_zone_1_time);
        } else if (settingsTimesMenu_currentMenu == 7) {
            eeprom_follow_wire_zone_2_time += 500;
            SaveIntToEEPROM(EEPROM_INDEX_FOLLOW_WIRE_ZONE_2_TIME, eeprom_follow_wire_zone_2_time);
        } else if (settingsTimesMenu_currentMenu == 8) {
            eeprom_wire_into_garden_time += 500;
            SaveIntToEEPROM(EEPROM_INDEX_WIRE_INTO_TO_GARDEN_TIME, eeprom_wire_into_garden_time);
        }

    } else if (MinusKey_pressed == 0) {
        delay(250);
        if (settingsTimesMenu_currentMenu == 0) {
            eeprom_exit_garage_backwards_time -= 500;
            SaveIntToEEPROM(EEPROM_INDEX_EXIT_GARAGE_BACKWARDS_TIME, eeprom_exit_garage_backwards_time);
        } else if (settingsTimesMenu_currentMenu == 1) {
            eeprom_random_rotate_time_min -= 500;
            SaveIntToEEPROM(EEPROM_INDEX_RANDOM_ROTATE_TIME_MIN, eeprom_random_rotate_time_min);
        } else if (settingsTimesMenu_currentMenu == 2) {
            eeprom_random_rotate_time_max -= 500;
            SaveIntToEEPROM(EEPROM_INDEX_RANDOM_ROTATE_TIME_MAX, eeprom_random_rotate_time_max);
        } else if (settingsTimesMenu_currentMenu == 3) {
            eeprom_rotate_from_exit_garage_to_wire_time -= 500;
            SaveIntToEEPROM(EEPROM_INDEX_ROTATE_FROM_EXIT_GARAGE_TO_WIRE_TIME, eeprom_rotate_from_exit_garage_to_wire_time);
        } else if (settingsTimesMenu_currentMenu == 4) {
            eeprom_rotate_from_wire_to_garden_time -= 500;
            SaveIntToEEPROM(EEPROM_INDEX_ROTATE_FROM_WIRE_TO_GARDEN_TIME, eeprom_rotate_from_wire_to_garden_time);
        } else if (settingsTimesMenu_currentMenu == 5) {
            eeprom_find_wire_search_time_max -= 500;
            SaveIntToEEPROM(EEPROM_INDEX_FIND_WIRE_SEARCH_TIME_MAX, eeprom_find_wire_search_time_max);
        } else if (settingsTimesMenu_currentMenu == 6) {
            eeprom_follow_wire_zone_1_time -= 500;
            SaveIntToEEPROM(EEPROM_INDEX_FOLLOW_WIRE_ZONE_1_TIME, eeprom_follow_wire_zone_1_time);
        } else if (settingsTimesMenu_currentMenu == 7) {
            eeprom_follow_wire_zone_2_time -= 500;
            SaveIntToEEPROM(EEPROM_INDEX_FOLLOW_WIRE_ZONE_2_TIME, eeprom_follow_wire_zone_2_time);
        } else if (settingsTimesMenu_currentMenu == 8) {
            eeprom_wire_into_garden_time -= 500;
            SaveIntToEEPROM(EEPROM_INDEX_WIRE_INTO_TO_GARDEN_TIME, eeprom_wire_into_garden_time);
        }
    } else if (StartKey_pressed == 0) {
        delay(250);
        settingsTimesMenu_currentMenu++;
        if (settingsTimesMenu_currentMenu >= ARRAY_SIZE(settingsTimesMenuNames)) {
            settingsTimesMenu_currentMenu = 0;
        }
    }
}

void settingsTimes_on_enter() {
    settingsTimesMenu_currentMenu = 0;
    ResetScrollRow0Text();
}

void settingsTimes() {
    String menuname = settingsTimesMenuNames[settingsTimesMenu_currentMenu];
    lcd.setCursor(0, 0);
    lcd.print(menuname + "                  ");

    lcd.setCursor(0, 1);
    lcd.write(126);
    if (settingsTimesMenu_currentMenu == 0) {
        lcd.print(" " + String(eeprom_exit_garage_backwards_time) + " ms           ");
    } else if (settingsTimesMenu_currentMenu == 1) {
        lcd.print(" " + String(eeprom_random_rotate_time_min) + " ms           ");
    } else if (settingsTimesMenu_currentMenu == 2) {
        lcd.print(" " + String(eeprom_random_rotate_time_max) + " ms           ");
    } else if (settingsTimesMenu_currentMenu == 3) {
        lcd.print(" " + String(eeprom_rotate_from_exit_garage_to_wire_time) + " ms           ");
    } else if (settingsTimesMenu_currentMenu == 4) {
        lcd.print(" " + String(eeprom_rotate_from_wire_to_garden_time) + " ms           ");
    } else if (settingsTimesMenu_currentMenu == 5) {
        lcd.print(" " + String(eeprom_find_wire_search_time_max) + " ms           ");
    } else if (settingsTimesMenu_currentMenu == 6) {
        lcd.print(" " + String(eeprom_follow_wire_zone_1_time) + " ms           ");
    } else if (settingsTimesMenu_currentMenu == 7) {
        lcd.print(" " + String(eeprom_follow_wire_zone_2_time) + " ms           ");
    } else if (settingsTimesMenu_currentMenu == 8) {
        lcd.print(" " + String(eeprom_wire_into_garden_time) + " ms           ");
    }
    read_settingsTimes_keys();
}

void settingsTimes_on_exit() {
    clearLCD();
}

State state_settingsTimesMenu(&settingsTimes_on_enter, &settingsTimes, &settingsTimes_on_exit);
