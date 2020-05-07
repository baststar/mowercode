#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/StateDockedMenu.h>
#include <vector>

using namespace std;

int dockedMenu_currentMenu = 0;
vector<String> dockedMenuNames = {"Mow from Zone 1", "Mow from Zone 2", "Quick mowing", "Tests", "Settings"};

void read_dockedMenu_keys() {

    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(BuildStateTransitionId(STATE_DOCKED_MENU, STATE_DOCKED), -1);
        return;
    } else if (PlusKey_pressed == 0) {
        delay(250);
        dockedMenu_currentMenu++;
        if (dockedMenu_currentMenu >= dockedMenuNames.size()) {
            dockedMenu_currentMenu = 0;
        }
    } else if (MinusKey_pressed == 0) {
        delay(250);
        dockedMenu_currentMenu--;
        if (dockedMenu_currentMenu < 0) {
            dockedMenu_currentMenu = dockedMenuNames.size() - 1;
        }
    } else if (StartKey_pressed == 0) {
        delay(250);
        if (dockedMenu_currentMenu == 0) {
            Trigger_FSM(BuildStateTransitionId(STATE_DOCKED_MENU, STATE_EXIT_GARAGE), FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_1);
            return;
        } else if (dockedMenu_currentMenu == 1) {
            Trigger_FSM(BuildStateTransitionId(STATE_DOCKED_MENU, STATE_EXIT_GARAGE), FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_2);
            return;
        } else if (dockedMenu_currentMenu == 2) {
            Trigger_FSM(BuildStateTransitionId(STATE_DOCKED_MENU, STATE_EXIT_GARAGE), FSMSEQUENCE_EXIT_GARAGE__RANDOM_ROTATE__MOWING);
            return;
        } else if (dockedMenu_currentMenu == 3) {
            Trigger_FSM(BuildStateTransitionId(STATE_DOCKED_MENU, STATE_TEST_MENU), -1);
            return;
        } else if (dockedMenu_currentMenu == 4) {
            Trigger_FSM(BuildStateTransitionId(STATE_DOCKED_MENU, STATE_SETTINGS_MENU), -1);
            return;
        }
    }
}

void dockedMenu_on_enter() {
    clearLCD();
    lcd.setCursor(0, 0);
    lcd.print("DOCKING-MENU                 ");
    delay(500);
    clearLCD();
    dockedMenu_currentMenu = 0;
}
void dockedMenu() {
    String menuname = GetMenuName(dockedMenuNames, dockedMenu_currentMenu);
    lcd.setCursor(0, 0);
    lcd.print(menuname + "           ");
    read_dockedMenu_keys();
}
void dockedMenu_on_exit() {
    clearLCD();
    lcd.setCursor(0, 0);
}
State state_dockedMenu(&dockedMenu_on_enter, &dockedMenu, &dockedMenu_on_exit);
