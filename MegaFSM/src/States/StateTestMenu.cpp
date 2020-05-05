#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/StateTestMenu.h>
#include <perimeter.h>


int testMenu_currentMenu = 0;
const int testMenuArraySize = 2;
String testMenuNames[testMenuArraySize] = {"Test Wire", "Test Relais"};

void read_testMenu_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(100);
        beforeMenuFSMEvent = currentFSMEvent;
        int stateId = (String(STATE_TEST_MENU) + String(9999) + String(STATE_DOCKED_MENU)).toInt();
        Trigger_FSM(stateId, -1);
        return;
    } else if (PlusKey_pressed == 0) {
        delay(100);
        testMenu_currentMenu++;
        if (testMenu_currentMenu >= testMenuArraySize) {
            testMenu_currentMenu = 0;
        }
    } else if (MinusKey_pressed == 0) {
        delay(100);
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
    delay(500);
    lcd.clear();
    testMenu_currentMenu = 0;
}
void testMenu() {

    read_testMenu_keys();
    Show_Test_Menu();

    if (testMenu_currentMenu == 0) {
        UpdatePerimeterStatus();
        lcd.setCursor(0, 1);
        lcd.print("mag: " + String(GetCurrentMagnitude()) + " i: " + String(MowerIsInsideWire()) + "           ");
    } else {
        lcd.setCursor(0, 1);
        lcd.print("                     ");
    }
}
void testMenu_on_exit() {
    lcd.clear();
}
State state_testMenu(&testMenu_on_enter, &testMenu, &testMenu_on_exit);
