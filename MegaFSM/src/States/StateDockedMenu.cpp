#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/StateDockedMenu.h>


int dockedMenu_currentMenu = 0;
const int dockedMenuArraySize = 4;
String dockedMenuNames[dockedMenuArraySize] = {"Mow from Zone 1", "Mow from Zone 2", "Quick mowing", "Tests"};

void read_dockedMenu_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        beforeMenuFSMEvent = currentFSMEvent;
        int stateId = (String(STATE_DOCKED_MENU) + String(9999) + String(STATE_DOCKED)).toInt();
        Trigger_FSM(stateId, -1);
        return;
    } else if (PlusKey_pressed == 0) {
        delay(100);
        dockedMenu_currentMenu++;
        if (dockedMenu_currentMenu >= dockedMenuArraySize) {
            dockedMenu_currentMenu = 0;
        }
    } else if (MinusKey_pressed == 0) {
        delay(100);
        dockedMenu_currentMenu--;
        if (dockedMenu_currentMenu < 0) {
            dockedMenu_currentMenu = dockedMenuArraySize - 1;
        }
    } else if (StartKey_pressed == 0) {
        delay(100);
        if (dockedMenu_currentMenu == 0) {
            int stateId = (String(STATE_DOCKED_MENU) + String(9999) + String(STATE_EXIT_GARAGE)).toInt();
            Trigger_FSM(stateId, FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_1);
            return;
        } else if (dockedMenu_currentMenu == 1) {
            int stateId = (String(STATE_DOCKED_MENU) + String(9999) + String(STATE_EXIT_GARAGE)).toInt();
            Trigger_FSM(stateId, FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_2);
            return;
        } else if (dockedMenu_currentMenu == 2) {
            int stateId = (String(STATE_DOCKED_MENU) + String(9999) + String(STATE_EXIT_GARAGE)).toInt();
            Trigger_FSM(stateId, FSMSEQUENCE_EXIT_GARAGE__RANDOM_ROTATE__MOWING);
            return;
        } else if (dockedMenu_currentMenu == 3) {
            int stateId = (String(STATE_DOCKED_MENU) + String(9999) + String(STATE_TEST_MENU)).toInt();
            Trigger_FSM(stateId, -1);
            return;
        }
    }
}

void dockedMenu_on_enter() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DOCKING-MENU                 ");
    delay(500);
    lcd.clear();
    dockedMenu_currentMenu = 0;
}
void dockedMenu() {
    read_dockedMenu_keys();
    Show_Docked_Menu();
}
void dockedMenu_on_exit() {
    lcd.clear();
}
State state_dockedMenu(&dockedMenu_on_enter, &dockedMenu, &dockedMenu_on_exit);
