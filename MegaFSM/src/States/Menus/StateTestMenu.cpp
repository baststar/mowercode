#include <Clock.h>
#include <Compass.h>
#include <CustomFunctions.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <Perimeter.h>
#include <States/FSMMower.h>
#include <States/FSMStates.h>
#include <States/Menus/StateTestMenu.h>
#include <VoltAmpRain.h>


int testMenu_currentMenu = 0;
String testMenuNames[] = {"Perimeter", "Compass", "Clock", "Temperature", "Amp/Volt/Rain"};

void read_testMenu_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMTransition = currentFSMTransition;
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
    testMenu_currentMenu = 0;
}

void testMenu() {
    String menuname = testMenuNames[testMenu_currentMenu];
    lcd.setCursor(0, 0);
    lcd.print(menuname);
    lcd.setCursor(0, 1);
    if (testMenu_currentMenu == 0) {
        lcd.setCursor(0, 0);
        String insideOutside = MowerIsInsideWire() == 0 ? "Outside" : "Inside";
        lcd.print(menuname + " " + insideOutside + "     ");
        lcd.setCursor(0, 1);
        lcd.print("L " + String(GetCurrentMagnitudeLeft()) + " R " + String(GetCurrentMagnitudeRight()) + "    ");
    } else if (testMenu_currentMenu == 1) {
        String heading = String(GetHeadingLoop());
        lcd.print(heading + "                    ");
    } else if (testMenu_currentMenu == 2) {
        lcd.print(GetDateTimeAsString() + "                  ");
        TestRTC();
    } else if (testMenu_currentMenu == 3) {
        lcd.print(GetTemperature() + " C                  ");
        TestRTC();
    } else if (testMenu_currentMenu == 4) {
        String itRains = IsRaining() == true ? "1" : "0";
        // lcd.print(String(GetAmps()) + " " + String(GetBatteryVolt()) + " " + itRains + "      ");
        lcd.print(String(GetAmps()) + " " + String(GetBatteryVolt()) + " " + String(GetRawRaining()) + "      ");
    } else {
        lcd.print("                     ");
    }

    read_testMenu_keys();
}
void testMenu_on_exit() {
    clearLCD();
}
State state_testMenu(&testMenu_on_enter, &testMenu, &testMenu_on_exit);
