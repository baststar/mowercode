#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/StateSettingsMotorspeedsMenu.h>
#include <vector>

using namespace std;


int settingsMotorspeedsMenu_currentMenu = 0;
vector<String> settingsMotorspeedsMenuNames = {"Motor Left", "Motor Right", "Blades", "Slow Speed"};

void read_settingsMotorspeeds_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(BuildStateTransitionId(STATE_SETTINGS_MOTORSPEED_MENU, STATE_SETTINGS_MENU), -1);
        return;
    } else if (PlusKey_pressed == 0) {
        delay(250);
        settingsMotorspeedsMenu_currentMenu++;
        if (settingsMotorspeedsMenu_currentMenu >= settingsMotorspeedsMenuNames.size()) {
            settingsMotorspeedsMenu_currentMenu = 0;
        }
    } else if (MinusKey_pressed == 0) {
        delay(250);
        settingsMotorspeedsMenu_currentMenu--;
        if (settingsMotorspeedsMenu_currentMenu < 0) {
            settingsMotorspeedsMenu_currentMenu = settingsMotorspeedsMenuNames.size() - 1;
        }
    } else if (StartKey_pressed == 0) {
        delay(250);
        if (settingsMotorspeedsMenu_currentMenu == 0) {
            // Trigger_FSM(BuildStateTransitionId(STATE_DOCKED_MENU, STATE_EXIT_GARAGE), FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_1);
            return;
        } else if (settingsMotorspeedsMenu_currentMenu == 1) {
            // Trigger_FSM(BuildStateTransitionId(STATE_DOCKED_MENU, STATE_EXIT_GARAGE), FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_2);
            return;
        } else if (settingsMotorspeedsMenu_currentMenu == 2) {
            // Trigger_FSM(BuildStateTransitionId(STATE_DOCKED_MENU, STATE_EXIT_GARAGE), FSMSEQUENCE_EXIT_GARAGE__RANDOM_ROTATE__MOWING);
            return;
        } else if (settingsMotorspeedsMenu_currentMenu == 3) {
            // Trigger_FSM(BuildStateTransitionId(STATE_DOCKED_MENU, STATE_TEST_MENU), -1);
            return;
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
    String menunameCurrent = GetMenuName(settingsMotorspeedsMenuNames, settingsMotorspeedsMenu_currentMenu);
    lcd.setCursor(0, 0);
    lcd.print(menunameCurrent + "           ");
    read_settingsMotorspeeds_keys();
}
void settingsMotorspeeds_on_exit() {
    clearLCD();
}
State state_settingsMotorspeedsMenu(&settingsMotorspeeds_on_enter, &settingsMotorspeeds, &settingsMotorspeeds_on_exit);
