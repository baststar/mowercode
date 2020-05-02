#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/StateDockedMenu.h>


void read_dockedMenu_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        trigger_fsm(FSMEVENT_DOCKED_MENU__TO__DOCKED);
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
    }
}

// DOCKEDMENU-STATE
void dockedMenu_on_enter() {
    dockedMenu_currentMenu = 0;
}
void dockedMenu() {
    read_dockedMenu_keys();
    Show_Docked_Menu();
}
void dockedMenu_on_exit() {
    dockedMenu_currentMenu = 0;
}
State state_dockedMenu(&dockedMenu_on_enter, &dockedMenu, &dockedMenu_on_exit);
