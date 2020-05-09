#include <CustomFunctions.h>
#include <EEPROMVariables.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/Menus/StateSettingsAlarmsMenu.h>
#include <config.h>


int settingsAlarmsMenu_currentMenu = 0;
String settingsAlarmsMenuNames[] = {"ALARM 1 ACTIVE", "ALARM 1 HOUR", "ALARM 1 MINUTE", "ALARM 2 ACTIVE", "ALARM 2 HOUR", "ALARM 2 MINUTE", "ALARM 3 ACTIVE", "ALARM 3 HOUR", "ALARM 3 MINUTE"};

void read_settingsAlarms_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMEvent = currentFSMEvent;
        TriggerFSM(STATE_SETTINGS_ALARMS_MENU, STATE_SETTINGS_MENU, -1);
        return;
    } else if (PlusKey_pressed == 0) {
        delay(250);
        if (settingsAlarmsMenu_currentMenu == 0) {
            eeprom_alarm_active_1 = eeprom_alarm_active_1 == 0 ? 1 : 0;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_ACTIVE_1, eeprom_alarm_active_1);
        } else if (settingsAlarmsMenu_currentMenu == 1) {
            eeprom_alarm_hour_1 += 1;
            if (eeprom_alarm_hour_1 > 23) {
                eeprom_alarm_hour_1 = 0;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_HOUR_1, eeprom_alarm_hour_1);
        } else if (settingsAlarmsMenu_currentMenu == 2) {
            eeprom_alarm_minute_1 += 1;
            if (eeprom_alarm_minute_1 > 59) {
                eeprom_alarm_minute_1 = 0;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_MINUTE_1, eeprom_alarm_minute_1);

        } else if (settingsAlarmsMenu_currentMenu == 3) {
            eeprom_alarm_active_2 = eeprom_alarm_active_2 == 0 ? 1 : 0;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_ACTIVE_2, eeprom_alarm_active_2);
        } else if (settingsAlarmsMenu_currentMenu == 4) {
            eeprom_alarm_hour_2 += 1;
            if (eeprom_alarm_hour_2 > 23) {
                eeprom_alarm_hour_2 = 0;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_HOUR_2, eeprom_alarm_hour_2);
        } else if (settingsAlarmsMenu_currentMenu == 5) {
            eeprom_alarm_minute_2 += 1;
            if (eeprom_alarm_minute_2 > 59) {
                eeprom_alarm_minute_2 = 0;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_MINUTE_2, eeprom_alarm_minute_2);

        } else if (settingsAlarmsMenu_currentMenu == 6) {
            eeprom_alarm_active_3 = eeprom_alarm_active_3 == 0 ? 1 : 0;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_ACTIVE_3, eeprom_alarm_active_3);
        } else if (settingsAlarmsMenu_currentMenu == 7) {
            eeprom_alarm_hour_3 += 1;
            if (eeprom_alarm_hour_3 > 23) {
                eeprom_alarm_hour_3 = 0;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_HOUR_3, eeprom_alarm_hour_3);
        } else if (settingsAlarmsMenu_currentMenu == 8) {
            eeprom_alarm_minute_3 += 1;
            if (eeprom_alarm_minute_3 > 59) {
                eeprom_alarm_minute_3 = 0;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_MINUTE_3, eeprom_alarm_minute_3);
        }

    } else if (MinusKey_pressed == 0) {
        delay(250);
        if (settingsAlarmsMenu_currentMenu == 0) {
            eeprom_alarm_active_1 = eeprom_alarm_active_1 == 0 ? 1 : 0;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_ACTIVE_1, eeprom_alarm_active_1);
        } else if (settingsAlarmsMenu_currentMenu == 1) {
            eeprom_alarm_hour_1 -= 1;
            if (eeprom_alarm_hour_1 > 23) { // unsigned int
                eeprom_alarm_hour_1 = 23;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_HOUR_1, eeprom_alarm_hour_1);
        } else if (settingsAlarmsMenu_currentMenu == 2) {
            eeprom_alarm_minute_1 -= 1;
            if (eeprom_alarm_minute_1 > 59) { // unsigned int
                eeprom_alarm_minute_1 = 59;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_MINUTE_1, eeprom_alarm_minute_1);

        } else if (settingsAlarmsMenu_currentMenu == 3) {
            eeprom_alarm_active_2 = eeprom_alarm_active_2 == 0 ? 1 : 0;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_ACTIVE_2, eeprom_alarm_active_2);
        } else if (settingsAlarmsMenu_currentMenu == 4) {
            eeprom_alarm_hour_2 -= 1;
            if (eeprom_alarm_hour_2 > 23) { // unsigned int
                eeprom_alarm_hour_2 = 23;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_HOUR_2, eeprom_alarm_hour_2);
        } else if (settingsAlarmsMenu_currentMenu == 5) {
            eeprom_alarm_minute_2 -= 1;
            if (eeprom_alarm_minute_2 > 59) { // unsigned int
                eeprom_alarm_minute_2 = 59;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_MINUTE_2, eeprom_alarm_minute_2);

        } else if (settingsAlarmsMenu_currentMenu == 6) {
            eeprom_alarm_active_3 = eeprom_alarm_active_3 == 0 ? 1 : 0;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_ACTIVE_3, eeprom_alarm_active_3);
        } else if (settingsAlarmsMenu_currentMenu == 7) {
            eeprom_alarm_hour_3 -= 1;
            if (eeprom_alarm_hour_3 > 23) { // unsigned int
                eeprom_alarm_hour_3 = 23;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_HOUR_3, eeprom_alarm_hour_3);
        } else if (settingsAlarmsMenu_currentMenu == 8) {
            eeprom_alarm_minute_3 -= 1;
            if (eeprom_alarm_minute_3 > 59) { // unsigned int
                eeprom_alarm_minute_3 = 59;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_MINUTE_3, eeprom_alarm_minute_3);
        }

    } else if (StartKey_pressed == 0) {
        delay(250);
        settingsAlarmsMenu_currentMenu++;
        if (settingsAlarmsMenu_currentMenu >= ARRAY_SIZE(settingsAlarmsMenuNames)) {
            settingsAlarmsMenu_currentMenu = 0;
        }
    }
}

void settingsAlarms_on_enter() {
    clearLCD();
    lcd.setCursor(0, 0);
    lcd.print("MOWTIMES                 ");
    delay(500);
    clearLCD();
    settingsAlarmsMenu_currentMenu = 0;
    ResetScrollRow0Text();
}

void settingsAlarms() {
    String menuname = settingsAlarmsMenuNames[settingsAlarmsMenu_currentMenu];
    lcd.setCursor(0, 0);
    lcd.print(menuname + "                  ");

    lcd.setCursor(0, 1);
    lcd.write(126);
    if (settingsAlarmsMenu_currentMenu == 0) {
        String alert1active = eeprom_alarm_active_1 == 1 ? "active" : "inactive";
        lcd.print(" " + alert1active + "            ");
    } else if (settingsAlarmsMenu_currentMenu == 1) {
        lcd.print("Hour " + String(eeprom_alarm_hour_1) + "            ");
    } else if (settingsAlarmsMenu_currentMenu == 2) {
        lcd.print("Minute " + String(eeprom_alarm_minute_1) + "            ");
    } else if (settingsAlarmsMenu_currentMenu == 3) {
        String alert2active = eeprom_alarm_active_2 == 1 ? "active" : "inactive";
        lcd.print(" " + alert2active + "            ");
    } else if (settingsAlarmsMenu_currentMenu == 4) {
        lcd.print("Hour " + String(eeprom_alarm_hour_2) + "            ");
    } else if (settingsAlarmsMenu_currentMenu == 5) {
        lcd.print("Minute " + String(eeprom_alarm_minute_2) + "            ");
    } else if (settingsAlarmsMenu_currentMenu == 6) {
        String alert3active = eeprom_alarm_active_3 == 1 ? "active" : "inactive";
        lcd.print(" " + alert3active + "            ");
    } else if (settingsAlarmsMenu_currentMenu == 7) {
        lcd.print("Hour " + String(eeprom_alarm_hour_3) + "            ");
    } else if (settingsAlarmsMenu_currentMenu == 8) {
        lcd.print("Minute " + String(eeprom_alarm_minute_3) + "            ");
    }

    read_settingsAlarms_keys();
}

void settingsAlarms_on_exit() {
    clearLCD();
}

State state_settingsAlarmsMenu(&settingsAlarms_on_enter, &settingsAlarms, &settingsAlarms_on_exit);
