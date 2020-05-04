#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/StateDockedMenu.h>


int dockedMenu_currentMenu = 0;
int dockedMenu_lastMenu = -1;
const int dockedMenuArraySize = 4;
String dockedMenuNames[dockedMenuArraySize] = {"Mow from Zone 1", "Mow from Zone 2", "Quick mowing", "Tests"};

void read_dockedMenu_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(FSMEVENT_DOCKED_MENU__TO__DOCKED, -1);
        return;
    } else if (PlusKey_pressed == 0) {
        dockedMenu_currentMenu++;
        if (dockedMenu_currentMenu >= dockedMenuArraySize) {
            dockedMenu_currentMenu = 0;
        }
    } else if (MinusKey_pressed == 0) {
        dockedMenu_currentMenu--;
        if (dockedMenu_currentMenu < 0) {
            dockedMenu_currentMenu = dockedMenuArraySize - 1;
        }
        delay(100);
    } else if (StartKey_pressed == 0) {

        if (dockedMenu_currentMenu == 0) {
            Trigger_FSM(FSMEVENT_DOCKED_MENU__TO__EXIT_GARAGE, FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_1);
            return;
        } else if (dockedMenu_currentMenu == 1) {
            Trigger_FSM(FSMEVENT_DOCKED_MENU__TO__EXIT_GARAGE, FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_2);
            return;
        } else if (dockedMenu_currentMenu == 2) {
            Trigger_FSM(FSMEVENT_DOCKED_MENU__TO__EXIT_GARAGE, FSMSEQUENCE_EXIT_GARAGE__RANDOM_ROTATE__MOWING);
            return;
        } else if (dockedMenu_currentMenu == 3) {
            Trigger_FSM(FSMEVENT_DOCKED_MENU__TO__TEST_MENU, -1);
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
    dockedMenu_lastMenu = -1;
}
void dockedMenu() {
    read_dockedMenu_keys();
    Show_Docked_Menu();
    if (dockedMenu_lastMenu != dockedMenu_currentMenu) {
        dockedMenu_lastMenu = dockedMenu_currentMenu;
        delay(200);
    }
}
void dockedMenu_on_exit() {
    lcd.clear();
}
State state_dockedMenu(&dockedMenu_on_enter, &dockedMenu, &dockedMenu_on_exit);
