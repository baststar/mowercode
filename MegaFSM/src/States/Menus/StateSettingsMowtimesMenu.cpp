#include <CustomFunctions.h>
#include <EEPROMVariables.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMStates.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/Menus/StateSettingsMowtimesMenu.h>
#include <config.h>


int settingsMowtimesMenu_currentMenu = 0;
String settingsMowtimesMenuNames[] = {"Quick mow", "Zone 1", "Zone 2"};

void read_settingsMowtimes_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMTransition = currentFSMTransition;
        TriggerFSM(STATE_SETTINGS_MOWTIMES_MENU, STATE_SETTINGS_MENU, -1);
        return;
    } else if (PlusKey_pressed == 0) {
        delay(250);
        if (settingsMowtimesMenu_currentMenu == 0) {
            eeprom_quick_mow_mowtime += 1;
            SaveIntToEEPROM(EEPROM_INDEX_QUICK_MOW_MOWTIME, eeprom_quick_mow_mowtime);
        } else if (settingsMowtimesMenu_currentMenu == 1) {
            eeprom_zone_1_mowtime += 1;
            SaveIntToEEPROM(EEPROM_INDEX_ZONE_1_MOWTIME, eeprom_zone_1_mowtime);
        } else if (settingsMowtimesMenu_currentMenu == 2) {
            eeprom_zone_2_mowtime += 1;
            SaveIntToEEPROM(EEPROM_INDEX_ZONE_2_MOWTIME, eeprom_zone_2_mowtime);
        }

    } else if (MinusKey_pressed == 0) {
        delay(250);
        if (settingsMowtimesMenu_currentMenu == 0) {
            eeprom_quick_mow_mowtime -= 1;
            SaveIntToEEPROM(EEPROM_INDEX_QUICK_MOW_MOWTIME, eeprom_quick_mow_mowtime);
        } else if (settingsMowtimesMenu_currentMenu == 1) {
            eeprom_zone_1_mowtime -= 1;
            SaveIntToEEPROM(EEPROM_INDEX_ZONE_1_MOWTIME, eeprom_zone_1_mowtime);
        } else if (settingsMowtimesMenu_currentMenu == 2) {
            eeprom_zone_2_mowtime -= 1;
            SaveIntToEEPROM(EEPROM_INDEX_ZONE_2_MOWTIME, eeprom_zone_2_mowtime);
        }

    } else if (StartKey_pressed == 0) {
        delay(250);
        settingsMowtimesMenu_currentMenu++;
        if (settingsMowtimesMenu_currentMenu >= ARRAY_SIZE(settingsMowtimesMenuNames)) {
            settingsMowtimesMenu_currentMenu = 0;
        }
    }
}

void settingsMowtimes_on_enter() {
    settingsMowtimesMenu_currentMenu = 0;
    ResetScrollRow0Text();
}

void settingsMowtimes() {
    String menuname = settingsMowtimesMenuNames[settingsMowtimesMenu_currentMenu];
    lcd.setCursor(0, 0);
    lcd.print(menuname + "                  ");

    lcd.setCursor(0, 1);
    lcd.write(126);
    if (settingsMowtimesMenu_currentMenu == 0) {
        lcd.print(" " + String(eeprom_quick_mow_mowtime) + " minutes           ");
    } else if (settingsMowtimesMenu_currentMenu == 1) {
        lcd.print(" " + String(eeprom_zone_1_mowtime) + " minutes           ");
    } else if (settingsMowtimesMenu_currentMenu == 2) {
        lcd.print(" " + String(eeprom_zone_2_mowtime) + " minutes           ");
    }
    read_settingsMowtimes_keys();
}

void settingsMowtimes_on_exit() {
    clearLCD();
}

State state_settingsMowtimesMenu(&settingsMowtimes_on_enter, &settingsMowtimes, &settingsMowtimes_on_exit);
