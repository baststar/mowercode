#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/StateSettingsTimesMenu.h>
#include <vector>

using namespace std;


int settingsTimesMenu_currentMenu = 0;
vector<String> settingsTimesMenuNames = {"Exit from garage time", "Random Rotate Min Time", "Random Rotate Max Time", "Rot. after Garage", "Rot. to Garden", "Find Wire Max", "Zone 1", "Zone 1",
                                         "Wire into Garden"};

void read_settingsTimes_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(BuildStateTransitionId(STATE_SETTINGS_TIMES_MENU, STATE_SETTINGS_MENU), -1);
        return;
    } else if (PlusKey_pressed == 0) {
        delay(250);
        settingsTimesMenu_currentMenu++;
        if (settingsTimesMenu_currentMenu >= settingsTimesMenuNames.size()) {
            settingsTimesMenu_currentMenu = 0;
        }
    } else if (MinusKey_pressed == 0) {
        delay(250);
        settingsTimesMenu_currentMenu--;
        if (settingsTimesMenu_currentMenu < 0) {
            settingsTimesMenu_currentMenu = settingsTimesMenuNames.size() - 1;
        }
    } else if (StartKey_pressed == 0) {
        delay(250);
        if (settingsTimesMenu_currentMenu == 0) {
            // Trigger_FSM(BuildStateTransitionId(STATE_DOCKED_MENU, STATE_EXIT_GARAGE), FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_1);
            return;
        } else if (settingsTimesMenu_currentMenu == 1) {
            // Trigger_FSM(BuildStateTransitionId(STATE_DOCKED_MENU, STATE_EXIT_GARAGE), FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_2);
            return;
        } else if (settingsTimesMenu_currentMenu == 2) {
            // Trigger_FSM(BuildStateTransitionId(STATE_DOCKED_MENU, STATE_EXIT_GARAGE), FSMSEQUENCE_EXIT_GARAGE__RANDOM_ROTATE__MOWING);
            return;
        } else if (settingsTimesMenu_currentMenu == 3) {
            // Trigger_FSM(BuildStateTransitionId(STATE_DOCKED_MENU, STATE_TEST_MENU), -1);
            return;
        }
    }
}

void settingsTimes_on_enter() {
    clearLCD();
    lcd.setCursor(0, 0);
    lcd.print("SETTINGS-MOTORSPEED                 ");
    delay(500);
    clearLCD();
    settingsTimesMenu_currentMenu = 0;
    ResetScrollRow0Text();
}

void settingsTimes() {
    String menuname = GetMenuName(settingsTimesMenuNames, settingsTimesMenu_currentMenu);
    lcd.setCursor(0, 0);
    lcd.print(menuname + "           ");
    read_settingsTimes_keys();
}

void settingsTimes_on_exit() {
    clearLCD();
}
State state_settingsTimesMenu(&settingsTimes_on_enter, &settingsTimes, &settingsTimes_on_exit);
