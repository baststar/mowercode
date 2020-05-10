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
#include <map>

uint8_t settingsAlarmsMenu_currentMenu = 0;
String settingsAlarmsMenuNames[] = {"Alarm 1 active", "Alarm 1 hour", "Alarm 1 minute", "Alarm 1 mode", "Alarm 2 active", "Alarm 2 hour",
                                    "Alarm 2 minute", "Alarm 2 mode", "Alarm 3 active", "Alarm 3 hour", "Alarm 3 minute", "Alarm 3 mode"};

std::map<uint16_t, String> alarmSequences = {{FSMSEQUENCE_ZONE_1, "ZONE 1"}, {FSMSEQUENCE_ZONE_2, "ZONE 2"}, {FSMSEQUENCE_QUICK_MOW, "QUICK MOW"}};

std::map<uint16_t, String>::iterator currentAlarmSequence1Iterator(alarmSequences.begin());
std::map<uint16_t, String>::iterator currentAlarmSequence2Iterator(alarmSequences.begin());
std::map<uint16_t, String>::iterator currentAlarmSequence3Iterator(alarmSequences.begin());

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
            // "ALARM 1 ACTIVE"
            eeprom_alarm_active_1 = eeprom_alarm_active_1 == 0 ? 1 : 0;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_ACTIVE_1, eeprom_alarm_active_1);
        } else if (settingsAlarmsMenu_currentMenu == 1) {
            // "ALARM 1 HOUR"
            eeprom_alarm_hour_1 += 1;
            if (eeprom_alarm_hour_1 > 23) {
                eeprom_alarm_hour_1 = 0;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_HOUR_1, eeprom_alarm_hour_1);
        } else if (settingsAlarmsMenu_currentMenu == 2) {
            // "ALARM 1 MINUTE"
            eeprom_alarm_minute_1 += 1;
            if (eeprom_alarm_minute_1 > 59) {
                eeprom_alarm_minute_1 = 0;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_MINUTE_1, eeprom_alarm_minute_1);
        } else if (settingsAlarmsMenu_currentMenu == 3) {
            // "ALARM 1 SEQUENCE"
            currentAlarmSequence1Iterator++;
            if (currentAlarmSequence1Iterator == alarmSequences.end()) {
                currentAlarmSequence1Iterator = alarmSequences.begin();
            }
            eeprom_alarm_sequence_1 = currentAlarmSequence1Iterator->first;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_SEQUENCE_1, eeprom_alarm_sequence_1);
        } else if (settingsAlarmsMenu_currentMenu == 4) {
            // "ALARM 2 ACTIVE"
            eeprom_alarm_active_2 = eeprom_alarm_active_2 == 0 ? 1 : 0;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_ACTIVE_2, eeprom_alarm_active_2);
        } else if (settingsAlarmsMenu_currentMenu == 5) {
            // "ALARM 2 HOUR"
            eeprom_alarm_hour_2 += 1;
            if (eeprom_alarm_hour_2 > 23) {
                eeprom_alarm_hour_2 = 0;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_HOUR_2, eeprom_alarm_hour_2);
        } else if (settingsAlarmsMenu_currentMenu == 6) {
            // "ALARM 2 MINUTE"
            eeprom_alarm_minute_2 += 1;
            if (eeprom_alarm_minute_2 > 59) {
                eeprom_alarm_minute_2 = 0;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_MINUTE_2, eeprom_alarm_minute_2);
        } else if (settingsAlarmsMenu_currentMenu == 7) {
            // "ALARM 2 SEQUENCE"
            currentAlarmSequence2Iterator++;
            if (currentAlarmSequence2Iterator == alarmSequences.end()) {
                currentAlarmSequence2Iterator = alarmSequences.begin();
            }
            eeprom_alarm_sequence_2 = currentAlarmSequence2Iterator->first;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_SEQUENCE_2, eeprom_alarm_sequence_2);
        } else if (settingsAlarmsMenu_currentMenu == 8) {
            // "ALARM 3 ACTIVE"
            eeprom_alarm_active_3 = eeprom_alarm_active_3 == 0 ? 1 : 0;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_ACTIVE_3, eeprom_alarm_active_3);
        } else if (settingsAlarmsMenu_currentMenu == 9) {
            // "ALARM 3 HOUR"
            eeprom_alarm_hour_3 += 1;
            if (eeprom_alarm_hour_3 > 23) {
                eeprom_alarm_hour_3 = 0;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_HOUR_3, eeprom_alarm_hour_3);
        } else if (settingsAlarmsMenu_currentMenu == 10) {
            // "ALARM 2 MINUTE"
            eeprom_alarm_minute_3 += 1;
            if (eeprom_alarm_minute_3 > 59) {
                eeprom_alarm_minute_3 = 0;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_MINUTE_3, eeprom_alarm_minute_3);
        } else if (settingsAlarmsMenu_currentMenu == 11) {
            // "ALARM 3 SEQUENCE"
            currentAlarmSequence3Iterator++;
            if (currentAlarmSequence3Iterator == alarmSequences.end()) {
                currentAlarmSequence3Iterator = alarmSequences.begin();
            }
            eeprom_alarm_sequence_3 = currentAlarmSequence3Iterator->first;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_SEQUENCE_3, eeprom_alarm_sequence_3);
        }

    } else if (MinusKey_pressed == 0) {
        delay(250);
        if (settingsAlarmsMenu_currentMenu == 0) {
            // "ALARM 1 ACTIVE"
            eeprom_alarm_active_1 = eeprom_alarm_active_1 == 0 ? 1 : 0;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_ACTIVE_1, eeprom_alarm_active_1);
        } else if (settingsAlarmsMenu_currentMenu == 1) {
            // "ALARM 1 HOUR"
            eeprom_alarm_hour_1 -= 1;
            if (eeprom_alarm_hour_1 > 23) { // unsigned int
                eeprom_alarm_hour_1 = 23;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_HOUR_1, eeprom_alarm_hour_1);
        } else if (settingsAlarmsMenu_currentMenu == 2) {
            // "ALARM 1 MINUTE"
            eeprom_alarm_minute_1 -= 1;
            if (eeprom_alarm_minute_1 > 59) { // unsigned int
                eeprom_alarm_minute_1 = 59;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_MINUTE_1, eeprom_alarm_minute_1);

        } else if (settingsAlarmsMenu_currentMenu == 3) {
            // "ALARM 1 SEQUENCE"
            if (currentAlarmSequence1Iterator == alarmSequences.begin()) {
                currentAlarmSequence1Iterator = alarmSequences.end();
                currentAlarmSequence1Iterator--;
            } else {
                currentAlarmSequence1Iterator--;
            }
            eeprom_alarm_sequence_1 = currentAlarmSequence1Iterator->first;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_SEQUENCE_1, eeprom_alarm_sequence_1);
        } else if (settingsAlarmsMenu_currentMenu == 4) {
            // "ALARM 2 ACTIVE"
            eeprom_alarm_active_2 = eeprom_alarm_active_2 == 0 ? 1 : 0;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_ACTIVE_2, eeprom_alarm_active_2);
        } else if (settingsAlarmsMenu_currentMenu == 5) {
            // "ALARM 2 HOUR"
            eeprom_alarm_hour_2 -= 1;
            if (eeprom_alarm_hour_2 > 23) { // unsigned int
                eeprom_alarm_hour_2 = 23;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_HOUR_2, eeprom_alarm_hour_2);
        } else if (settingsAlarmsMenu_currentMenu == 6) {
            // "ALARM 2 MINUTE"
            eeprom_alarm_minute_2 -= 1;
            if (eeprom_alarm_minute_2 > 59) { // unsigned int
                eeprom_alarm_minute_2 = 59;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_MINUTE_2, eeprom_alarm_minute_2);

        } else if (settingsAlarmsMenu_currentMenu == 7) {
            // "ALARM 2 SEQUENCE"
            if (currentAlarmSequence2Iterator == alarmSequences.begin()) {
                currentAlarmSequence2Iterator = alarmSequences.end();
                currentAlarmSequence2Iterator--;
            } else {
                currentAlarmSequence2Iterator--;
            }
            eeprom_alarm_sequence_2 = currentAlarmSequence2Iterator->first;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_SEQUENCE_2, eeprom_alarm_sequence_2);
        } else if (settingsAlarmsMenu_currentMenu == 8) {
            // "ALARM 3 ACTIVE"
            eeprom_alarm_active_3 = eeprom_alarm_active_3 == 0 ? 1 : 0;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_ACTIVE_3, eeprom_alarm_active_3);
        } else if (settingsAlarmsMenu_currentMenu == 9) {
            // "ALARM 3 HOUR"
            eeprom_alarm_hour_3 -= 1;
            if (eeprom_alarm_hour_3 > 23) { // unsigned int
                eeprom_alarm_hour_3 = 23;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_HOUR_3, eeprom_alarm_hour_3);
        } else if (settingsAlarmsMenu_currentMenu == 10) {
            // "ALARM 3 MINUTE"
            eeprom_alarm_minute_3 -= 1;
            if (eeprom_alarm_minute_3 > 59) { // unsigned int
                eeprom_alarm_minute_3 = 59;
            }
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_MINUTE_3, eeprom_alarm_minute_3);
        } else if (settingsAlarmsMenu_currentMenu == 11) {
            // "ALARM 3 SEQUENCE"
            if (currentAlarmSequence3Iterator == alarmSequences.begin()) {
                currentAlarmSequence3Iterator = alarmSequences.end();
                currentAlarmSequence3Iterator--;
            } else {
                currentAlarmSequence3Iterator--;
            }
            eeprom_alarm_sequence_3 = currentAlarmSequence3Iterator->first;
            SaveIntToEEPROM(EEPROM_INDEX_ALARM_SEQUENCE_3, eeprom_alarm_sequence_3);
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
    currentAlarmSequence1Iterator = alarmSequences.find(eeprom_alarm_sequence_1);
    currentAlarmSequence2Iterator = alarmSequences.find(eeprom_alarm_sequence_2);
    currentAlarmSequence3Iterator = alarmSequences.find(eeprom_alarm_sequence_3);
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
        lcd.print(" Hour " + String(eeprom_alarm_hour_1) + "            ");
    } else if (settingsAlarmsMenu_currentMenu == 2) {
        lcd.print(" Minute " + String(eeprom_alarm_minute_1) + "            ");
    } else if (settingsAlarmsMenu_currentMenu == 3) {

        lcd.print(" Mode: " + currentAlarmSequence1Iterator->second + "            ");

    } else if (settingsAlarmsMenu_currentMenu == 4) {
        String alert2active = eeprom_alarm_active_2 == 1 ? "active" : "inactive";
        lcd.print(" " + alert2active + "            ");
    } else if (settingsAlarmsMenu_currentMenu == 5) {
        lcd.print(" Hour " + String(eeprom_alarm_hour_2) + "            ");
    } else if (settingsAlarmsMenu_currentMenu == 6) {
        lcd.print(" Minute " + String(eeprom_alarm_minute_2) + "            ");
    } else if (settingsAlarmsMenu_currentMenu == 7) {

        lcd.print(" Mode " + currentAlarmSequence2Iterator->second + "            ");

    } else if (settingsAlarmsMenu_currentMenu == 8) {
        String alert3active = eeprom_alarm_active_3 == 1 ? "active" : "inactive";
        lcd.print(" " + alert3active + "            ");
    } else if (settingsAlarmsMenu_currentMenu == 9) {
        lcd.print(" Hour " + String(eeprom_alarm_hour_3) + "            ");
    } else if (settingsAlarmsMenu_currentMenu == 10) {
        lcd.print(" Minute " + String(eeprom_alarm_minute_3) + "            ");
    } else if (settingsAlarmsMenu_currentMenu == 11) {

        lcd.print(" Mode " + currentAlarmSequence3Iterator->second + "            ");
    }

    read_settingsAlarms_keys();
}

void settingsAlarms_on_exit() {
    clearLCD();
}

State state_settingsAlarmsMenu(&settingsAlarms_on_enter, &settingsAlarms, &settingsAlarms_on_exit);
