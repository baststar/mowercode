#include <CustomFunctions.h>
#include <EEPROMVariables.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/Menus/StateSettingsMenu.h>


int settingsMenu_currentMenu = 0;

String settingsMenuNames[] = {"Motorspeeds...", "Times...", "Perimeter...", "Reset EEPROM"};

void read_settingsMenu_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(BuildStateTransitionId(STATE_SETTINGS_MENU, STATE_DOCKED_MENU), -1);
        return;
    } else if (PlusKey_pressed == 0) {
        delay(250);
        settingsMenu_currentMenu++;
        if (settingsMenu_currentMenu >= ARRAY_SIZE(settingsMenuNames)) {
            settingsMenu_currentMenu = 0;
        }
    } else if (MinusKey_pressed == 0) {
        delay(250);
        settingsMenu_currentMenu--;
        if (settingsMenu_currentMenu < 0) {
            settingsMenu_currentMenu = ARRAY_SIZE(settingsMenuNames) - 1;
        }
    } else if (StartKey_pressed == 0) {
        delay(250);
        if (settingsMenu_currentMenu == 0) {
            Trigger_FSM(BuildStateTransitionId(STATE_SETTINGS_MENU, STATE_SETTINGS_MOTORSPEED_MENU), -1);
            return;
        } else if (settingsMenu_currentMenu == 1) {
            Trigger_FSM(BuildStateTransitionId(STATE_SETTINGS_MENU, STATE_SETTINGS_TIMES_MENU), -1);
            return;
        } else if (settingsMenu_currentMenu == 2) {
            Trigger_FSM(BuildStateTransitionId(STATE_SETTINGS_MENU, STATE_SETTINGS_PERIMETER_MENU), -1);
            return;
        } else if (settingsMenu_currentMenu == 3) {
            ResetEEPROM();
            Trigger_FSM(BuildStateTransitionId(STATE_SETTINGS_MENU, STATE_DOCKED), -1);
            return;
        }
    }
}

void settingsMenu_on_enter() {
    clearLCD();
    lcd.setCursor(0, 0);
    lcd.print("SETTINGS                 ");
    delay(500);
    clearLCD();
    settingsMenu_currentMenu = 0;
}
void settingsMenu() {
    String menuname = settingsMenuNames[settingsMenu_currentMenu];
    lcd.setCursor(0, 0);

    if (settingsMenu_currentMenu == 3) {
        lcd.write(126);
        lcd.print(" ");
    }

    lcd.print(menuname + "           ");
    read_settingsMenu_keys();
}
void settingsMenu_on_exit() {
    clearLCD();
    lcd.setCursor(0, 0);
}
State state_settingsMenu(&settingsMenu_on_enter, &settingsMenu, &settingsMenu_on_exit);
