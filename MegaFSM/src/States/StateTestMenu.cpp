#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/StateTestMenu.h>

int testMenu_currentMenu = 0;
int testMenu_lastMenu = -1;
const int testMenuArraySize = 2;
String testMenuNames[testMenuArraySize] = {"Test Wire", "Test Relais"};

void read_testMenu_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(FSMEVENT_TEST_MENU__TO__DOCKED_MENU, -1);
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

void testMenu_on_enter() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TEST-MENU                   ");
    delay(1000);
    lcd.clear();
    testMenu_currentMenu = 0;
    testMenu_lastMenu = -1;
}
void testMenu() {
    read_testMenu_keys();
    Show_Test_Menu();
}
void testMenu_on_exit() {
    lcd.clear();
}
State state_testMenu(&testMenu_on_enter, &testMenu, &testMenu_on_exit);
