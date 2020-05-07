#include <CustomFunctions.h>
#include <EEPROMVariables.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/Menus/StateSettingsMotorspeedsMenu.h>
#include <config.h>

int settingsMotorspeedsMenu_currentMenu = 0;
String settingsMotorspeedsMenuNames[] = {"Motor Left", "Motor Right", "Slow Speed", "Blades"};

void read_settingsMotorspeeds_keys() {

    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(BuildStateTransitionId(STATE_SETTINGS_MOTORSPEED_MENU, STATE_SETTINGS_MENU), -1);
        return;
    } else if (PlusKey_pressed == 0) {
        delay(250);

        if (settingsMotorspeedsMenu_currentMenu == 0) {
            // Motor Left +
            eeprom_pwm_maxspeed_left++;
            if (eeprom_pwm_maxspeed_left > 255) {
                eeprom_pwm_maxspeed_left = 0;
            }
            SaveIntToEEPROM(EEPROM_INDEX_PWM_MAXSPEED_LEFT, eeprom_pwm_maxspeed_left);
        } else if (settingsMotorspeedsMenu_currentMenu == 1) {
            // Motor Right +
            eeprom_pwm_maxspeed_right++;
            if (eeprom_pwm_maxspeed_right > 255) {
                eeprom_pwm_maxspeed_right = 0;
            }
            SaveIntToEEPROM(EEPROM_INDEX_PWM_MAXSPEED_RIGHT, eeprom_pwm_maxspeed_right);
        } else if (settingsMotorspeedsMenu_currentMenu == 2) {
            // Slow Speed +
            eeprom_pwm_slowspeed++;
            if (eeprom_pwm_slowspeed > 255) {
                eeprom_pwm_slowspeed = 0;
            }
            SaveIntToEEPROM(EEPROM_INDEX_PWM_SLOWSPEED, eeprom_pwm_slowspeed);
        } else if (settingsMotorspeedsMenu_currentMenu == 3) {
            // Blades +
            eeprom_pwm_bladespeed++;
            if (eeprom_pwm_bladespeed > 255) {
                eeprom_pwm_bladespeed = 0;
            }
            SaveIntToEEPROM(EEPROM_INDEX_PWM_BLADESPEED, eeprom_pwm_bladespeed);
        }

    } else if (MinusKey_pressed == 0) {
        delay(250);

        if (settingsMotorspeedsMenu_currentMenu == 0) {
            // Motor Left -
            eeprom_pwm_maxspeed_left--;
            if (eeprom_pwm_maxspeed_left > 255) {
                eeprom_pwm_maxspeed_left = 255;
            }
            SaveIntToEEPROM(EEPROM_INDEX_PWM_MAXSPEED_LEFT, eeprom_pwm_maxspeed_left);
        } else if (settingsMotorspeedsMenu_currentMenu == 1) {
            // Motor Right -
            eeprom_pwm_maxspeed_right--;
            if (eeprom_pwm_maxspeed_right > 255) {
                eeprom_pwm_maxspeed_right = 255;
            }
            SaveIntToEEPROM(EEPROM_INDEX_PWM_MAXSPEED_RIGHT, eeprom_pwm_maxspeed_right);
        } else if (settingsMotorspeedsMenu_currentMenu == 2) {
            // Slow Speed -
            eeprom_pwm_slowspeed--;
            if (eeprom_pwm_slowspeed > 255) {
                eeprom_pwm_slowspeed = 255;
            }
            SaveIntToEEPROM(EEPROM_INDEX_PWM_SLOWSPEED, eeprom_pwm_slowspeed);
        } else if (settingsMotorspeedsMenu_currentMenu == 3) {
            // Blades -
            eeprom_pwm_bladespeed--;
            if (eeprom_pwm_bladespeed > 255) {
                eeprom_pwm_bladespeed = 255;
            }
            SaveIntToEEPROM(EEPROM_INDEX_PWM_BLADESPEED, eeprom_pwm_bladespeed);
        }

    } else if (StartKey_pressed == 0) {
        delay(250);
        settingsMotorspeedsMenu_currentMenu++;
        if (settingsMotorspeedsMenu_currentMenu >= ARRAY_SIZE(settingsMotorspeedsMenuNames)) {
            settingsMotorspeedsMenu_currentMenu = 0;
        }
    }
}

void settingsMotorspeeds_on_enter() {
    clearLCD();
    lcd.setCursor(0, 0);
    lcd.print("SETTINGS-MOTORSPEED                 ");
    delay(500);
    clearLCD();
    settingsMotorspeedsMenu_currentMenu = 0;
}
void settingsMotorspeeds() {
    String menunameCurrent = settingsMotorspeedsMenuNames[settingsMotorspeedsMenu_currentMenu];
    lcd.setCursor(0, 0);
    lcd.print(menunameCurrent + "           ");
    lcd.setCursor(0, 1);
    if (settingsMotorspeedsMenu_currentMenu == 0) {
        lcd.write(126);
        lcd.print(" " + String(eeprom_pwm_maxspeed_left) + "           ");
    } else if (settingsMotorspeedsMenu_currentMenu == 1) {
        lcd.write(126);
        lcd.print(" " + String(eeprom_pwm_maxspeed_right) + "           ");
    } else if (settingsMotorspeedsMenu_currentMenu == 2) {
        lcd.write(126);
        lcd.print(" " + String(eeprom_pwm_slowspeed) + "           ");
    } else if (settingsMotorspeedsMenu_currentMenu == 3) {
        lcd.write(126);
        lcd.print(" " + String(eeprom_pwm_bladespeed) + "           ");
    }
    read_settingsMotorspeeds_keys();
}
void settingsMotorspeeds_on_exit() {
    clearLCD();
}
State state_settingsMotorspeedsMenu(&settingsMotorspeeds_on_enter, &settingsMotorspeeds, &settingsMotorspeeds_on_exit);
