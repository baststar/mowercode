#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/StateTestMenu.h>
#include <perimeter.h>
#include <vector>

using namespace std;

int testMenu_currentMenu = 0;
vector<String> testMenuNames = {"Test Wire", "Test Relais"};

void read_testMenu_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(BuildStateTransitionId(STATE_TEST_MENU, STATE_DOCKED_MENU), -1);
    } else if (PlusKey_pressed == 0) {
        delay(250);
        testMenu_currentMenu++;
        if (testMenu_currentMenu >= testMenuNames.size()) {
            testMenu_currentMenu = 0;
        }
    } else if (MinusKey_pressed == 0) {
        delay(250);
        testMenu_currentMenu--;
        if (testMenu_currentMenu < 0) {
            testMenu_currentMenu = testMenuNames.size() - 1;
        }
    }
}

void testMenu_on_enter() {
    clearLCD();
    lcd.setCursor(0, 0);
    lcd.print("TEST-MENU                   ");
    delay(500);
    clearLCD();
    testMenu_currentMenu = 0;
}

void testMenu() {
    String menuname = GetMenuName(testMenuNames, testMenu_currentMenu);
    lcd.setCursor(0, 0);
    lcd.print(menuname + "           ");

    if (testMenu_currentMenu == 0) {
        UpdatePerimeterStatus();
        lcd.setCursor(0, 1);
        lcd.print("mag: " + String(GetCurrentMagnitude()) + " i: " + String(MowerIsInsideWire()) + "           ");
    } else {
        lcd.setCursor(0, 1);
        lcd.print("                     ");
    }
    
    read_testMenu_keys();
}
void testMenu_on_exit() {
    clearLCD();
}
State state_testMenu(&testMenu_on_enter, &testMenu, &testMenu_on_exit);
