#include <CustomFunctions.h>
#include <EEPROMVariables.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/Menus/StateSettingsMowtimesMenu.h>
#include <config.h>


int settingsMowtimesMenu_currentMenu = 0;
String settingsMowtimesMenuNames[] = {"QUICK MOW", "ZONE 1/2", "ALARM 1", "ALARM 2", "ALARM 3"};

void read_settingsMowtimes_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMEvent = currentFSMEvent;
        TriggerFSM(STATE_SETTINGS_MOWTIMES_MENU, STATE_SETTINGS_MENU, -1);
        return;
    } else if (PlusKey_pressed == 0) {
        delay(250);
        if (settingsMowtimesMenu_currentMenu == 0) {
            eeprom_quick_mow_mowtime += 1;
            SaveIntToEEPROM(EEPROM_INDEX_QUICK_MOW_MOWTIME, eeprom_quick_mow_mowtime);
        } else if (settingsMowtimesMenu_currentMenu == 1) {
            eeprom_exit_garage_mowtime += 1;
            SaveIntToEEPROM(EEPROM_INDEX_EXIT_GARAGE_MOWTIME, eeprom_exit_garage_mowtime);
        } else if (settingsMowtimesMenu_currentMenu == 2) {
            eeprom_alarm_mowtime_1 += 1;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_MOWTIME_1, eeprom_alarm_mowtime_1);
        } else if (settingsMowtimesMenu_currentMenu == 3) {
            eeprom_alarm_mowtime_2 += 1;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_MOWTIME_2, eeprom_alarm_mowtime_2);
        } else if (settingsMowtimesMenu_currentMenu == 4) {
            eeprom_alarm_mowtime_3 += 1;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_MOWTIME_3, eeprom_alarm_mowtime_3);
        }

    } else if (MinusKey_pressed == 0) {
        delay(250);
        if (settingsMowtimesMenu_currentMenu == 0) {
            eeprom_quick_mow_mowtime -= 1;
            SaveIntToEEPROM(EEPROM_INDEX_QUICK_MOW_MOWTIME, eeprom_quick_mow_mowtime);
        } else if (settingsMowtimesMenu_currentMenu == 1) {
            eeprom_exit_garage_mowtime -= 1;
            SaveIntToEEPROM(EEPROM_INDEX_EXIT_GARAGE_MOWTIME, eeprom_exit_garage_mowtime);
        } else if (settingsMowtimesMenu_currentMenu == 2) {
            eeprom_alarm_mowtime_1 -= 1;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_MOWTIME_1, eeprom_alarm_mowtime_1);
        } else if (settingsMowtimesMenu_currentMenu == 3) {
            eeprom_alarm_mowtime_2 -= 1;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_MOWTIME_2, eeprom_alarm_mowtime_2);
        } else if (settingsMowtimesMenu_currentMenu == 4) {
            eeprom_alarm_mowtime_3 -= 1;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_MOWTIME_3, eeprom_alarm_mowtime_3);
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
    clearLCD();
    lcd.setCursor(0, 0);
    lcd.print("MOWTIMES                 ");
    delay(500);
    clearLCD();
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
        lcd.print(" " + String(eeprom_exit_garage_mowtime) + " minutes           ");
    } else if (settingsMowtimesMenu_currentMenu == 2) {
        lcd.print(" " + String(eeprom_alarm_mowtime_1) + " minutes           ");
    } else if (settingsMowtimesMenu_currentMenu == 3) {
        lcd.print(" " + String(eeprom_alarm_mowtime_2) + " minutes           ");
    } else if (settingsMowtimesMenu_currentMenu == 4) {
        lcd.print(" " + String(eeprom_alarm_mowtime_3) + " minutes           ");
    }
    read_settingsMowtimes_keys();
}

void settingsMowtimes_on_exit() {
    clearLCD();
}

State state_settingsMowtimesMenu(&settingsMowtimes_on_enter, &settingsMowtimes, &settingsMowtimes_on_exit);
