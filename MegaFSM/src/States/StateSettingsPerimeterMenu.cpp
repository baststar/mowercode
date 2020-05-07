#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/StateSettingsPerimeterMenu.h>
#include <vector>

using namespace std;


int settingsPerimeterMenu_currentMenu = 0;
vector<String> settingsPerimeterMenuNames = {"CW from Grarage", "Mag inside", "Mag outside", "Same site time on wire"};

void read_settingsPerimeter_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(BuildStateTransitionId(STATE_SETTINGS_PERIMETER_MENU, STATE_SETTINGS_MENU), -1);
        return;
    } else if (PlusKey_pressed == 0) {
        delay(250);
        settingsPerimeterMenu_currentMenu++;
        if (settingsPerimeterMenu_currentMenu >= settingsPerimeterMenuNames.size()) {
            settingsPerimeterMenu_currentMenu = 0;
        }
    } else if (MinusKey_pressed == 0) {
        delay(250);
        settingsPerimeterMenu_currentMenu--;
        if (settingsPerimeterMenu_currentMenu < 0) {
            settingsPerimeterMenu_currentMenu = settingsPerimeterMenuNames.size() - 1;
        }
    } else if (StartKey_pressed == 0) {
        delay(250);
        if (settingsPerimeterMenu_currentMenu == 0) {
            // Trigger_FSM(BuildStateTransitionId(STATE_DOCKED_MENU, STATE_EXIT_GARAGE), FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_1);
            return;
        } else if (settingsPerimeterMenu_currentMenu == 1) {
            // Trigger_FSM(BuildStateTransitionId(STATE_DOCKED_MENU, STATE_EXIT_GARAGE), FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_2);
            return;
        } else if (settingsPerimeterMenu_currentMenu == 2) {
            // Trigger_FSM(BuildStateTransitionId(STATE_DOCKED_MENU, STATE_EXIT_GARAGE), FSMSEQUENCE_EXIT_GARAGE__RANDOM_ROTATE__MOWING);
            return;
        } else if (settingsPerimeterMenu_currentMenu == 3) {
            // Trigger_FSM(BuildStateTransitionId(STATE_DOCKED_MENU, STATE_TEST_MENU), -1);
            return;
        }
    }
}

void settingsPerimeter_on_enter() {
    clearLCD();
    lcd.setCursor(0, 0);
    lcd.print("SETTINGS-MOTORSPEED                 ");
    delay(500);
    clearLCD();
    settingsPerimeterMenu_currentMenu = 0;
}
void settingsPerimeter() {
    String menuname = GetMenuName(settingsPerimeterMenuNames, settingsPerimeterMenu_currentMenu);
    lcd.setCursor(0, 0);
    lcd.print(menuname + "           ");
    read_settingsPerimeter_keys();
}
void settingsPerimeter_on_exit() {
    clearLCD();
}
State state_settingsPerimeterMenu(&settingsPerimeter_on_enter, &settingsPerimeter, &settingsPerimeter_on_exit);
