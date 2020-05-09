#include <Clock.h>
#include <Compass.h>
#include <CustomFunctions.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/Menus/StateTestMenu.h>
#include <perimeter.h>



int testMenu_currentMenu = 0;
String testMenuNames[] = {"Perimeter", "Compass", "Clock"};

void read_testMenu_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMEvent = currentFSMEvent;
        TriggerFSM(STATE_TEST_MENU, STATE_DOCKED_MENU, -1);
    } else if (PlusKey_pressed == 0) {
        delay(250);
        testMenu_currentMenu++;
        if (testMenu_currentMenu >= ARRAY_SIZE(testMenuNames)) {
            testMenu_currentMenu = 0;
        }
    } else if (MinusKey_pressed == 0) {
        delay(250);
        testMenu_currentMenu--;
        if (testMenu_currentMenu < 0) {
            testMenu_currentMenu = ARRAY_SIZE(testMenuNames) - 1;
        }
    }
}

void testMenu_on_enter() {
    clearLCD();
    lcd.setCursor(0, 0);
    lcd.print("TESTS                  ");
    delay(500);
    clearLCD();
    testMenu_currentMenu = 0;
}

void testMenu() {
    String menuname = testMenuNames[testMenu_currentMenu];
    lcd.setCursor(0, 0);
    lcd.print(menuname + "           ");
    lcd.setCursor(0, 1);
    if (testMenu_currentMenu == 0) {
        UpdatePerimeterStatus();
        String insideOutside = MowerIsInsideWire() == 0 ? "Out" : "In";
        lcd.print(insideOutside + " mag: " + String(GetCurrentMagnitude()) + "           ");
    } else if (testMenu_currentMenu == 1) {
        String heading = String(GetHeading());
        lcd.print(heading + "                    ");
    } else if (testMenu_currentMenu == 2) {
        lcd.print(GetDateTimeAsString() + "                  ");
        TestRTC();
    } else {
        lcd.print("                     ");
    }

    read_testMenu_keys();
}
void testMenu_on_exit() {
    clearLCD();
}
State state_testMenu(&testMenu_on_enter, &testMenu, &testMenu_on_exit);
