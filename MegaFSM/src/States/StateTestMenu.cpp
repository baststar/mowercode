#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/StateTestMenu.h>


void read_testMenu_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        // trigger_fsm(EVENT_DOCKED_MENU__TO__DOCKED);
    } else if (PlusKey_pressed == 0) {
        testMenu_currentMenu++;
        if (testMenu_currentMenu >= testMenuArraySize) {
            testMenu_currentMenu = 0;
        }
    } else if (MinusKey_pressed == 0) {
        testMenu_currentMenu--;
        if (testMenu_currentMenu < 0) {
            testMenu_currentMenu = testMenuArraySize - 1;
        }
    }
}

// DOCKEDMENU-STATE
void testMenu_on_enter() {
    testMenu_currentMenu = 0;
}
void testMenu() {
    read_testMenu_keys();
    Show_Test_Menu();
}
void testMenu_on_exit() {
    testMenu_currentMenu = 0;
}
State state_testMenu(&testMenu_on_enter, &testMenu, &testMenu_on_exit);
