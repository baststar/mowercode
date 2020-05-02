#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/StateParkedMenu.h>


void read_parkedMenu_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        trigger_fsm(FSMEVENT_DOCKED_MENU__TO__DOCKED);
    } else if (PlusKey_pressed == 0) {
        parkedMenu_currentMenu++;
        if (parkedMenu_currentMenu >= parkedMenuArraySize) {
            parkedMenu_currentMenu = 0;
        }
    } else if (MinusKey_pressed == 0) {
        parkedMenu_currentMenu--;
        if (parkedMenu_currentMenu < 0) {
            parkedMenu_currentMenu = parkedMenuArraySize - 1;
        }
    }
}

// DOCKEDMENU-STATE
void parkedMenu_on_enter() {
    parkedMenu_currentMenu = 0;
}
void parkedMenu() {
    read_parkedMenu_keys();
    Show_Docked_Menu();
}
void parkedMenu_on_exit() {
    parkedMenu_currentMenu = 0;
}
State state_parkedMenu(&parkedMenu_on_enter, &parkedMenu, &parkedMenu_on_exit);
