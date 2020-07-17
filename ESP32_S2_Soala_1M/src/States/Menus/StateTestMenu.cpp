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
#include <TxRxFunctions.h>
#include <VoltAmpRain.h>
#include <config.h>


int testMenu_currentMenu = 0;
String testMenuNames[] = {"Perimeter", "Compass", "Clock", "Temperature", "Amp/Volt/Rain", "Relais", "Motors", "Blade"};

unsigned long lastMillisRelaistest = 0;
unsigned long relaisTestInterval = 1000;
int relaisStatus = 0;

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

    if (testMenu_currentMenu == 0) {
        ReceiveSerialData();
        lcd.setCursor(0, 0);
        String insideOutside = MowerIsInsideWire() == 0 ? "Outside" : "Inside";
        lcd.print(menuname + " " + insideOutside + "            ");
        lcd.setCursor(0, 1);
        lcd.print("L " + String(GetCurrentMagnitudeLeft()) + " R " + String(GetCurrentMagnitudeRight()) + "        ");
        MotorAction_StopMotors();
        RelaisOff();
        MotorAction_StopBlades();
    } else if (testMenu_currentMenu == 1) {
        String heading = String(GetHeadingLoop());
        lcd.setCursor(0, 1);
        lcd.print(heading + "                    ");
        lcd.setCursor(0, 0);
        lcd.print(menuname + "                  ");
        MotorAction_StopMotors();
        RelaisOff();
        MotorAction_StopBlades();
    } else if (testMenu_currentMenu == 2) {
        lcd.setCursor(0, 1);
        lcd.print(GetDateTimeAsString() + "                  ");
        lcd.setCursor(0, 0);
        lcd.print(menuname + "                  ");
        TestRTC();
        MotorAction_StopMotors();
        RelaisOff();
        MotorAction_StopBlades();
    } else if (testMenu_currentMenu == 3) {
        lcd.setCursor(0, 1);
        lcd.print(GetTemperature() + " C                  ");
        lcd.setCursor(0, 0);
        lcd.print(menuname + "                  ");
        TestRTC();
        MotorAction_StopMotors();
        RelaisOff();
        MotorAction_StopBlades();
    } else if (testMenu_currentMenu == 4) {
        String itRains = IsRaining() == true ? "1" : "0";
        // lcd.print(String(GetAmps()) + " " + String(GetBatteryVolt()) + " " + itRains + "      ");
        lcd.setCursor(0, 1);
        lcd.print(String(GetAmps()) + " " + String(GetBatteryVolt()) + " " + String(GetRawRaining()) + "           ");
        lcd.setCursor(0, 0);
        lcd.print(menuname + "                  ");
        MotorAction_StopMotors();
        RelaisOff();
        MotorAction_StopBlades();
    } else if (testMenu_currentMenu == 5) {
        if (millis() - lastMillisRelaistest > relaisTestInterval) {
            relaisStatus = relaisStatus == 0 ? 1 : 0;
            if (relaisStatus == 1) {
                RelaisOn();
            } else {
                RelaisOff();
            }
            lastMillisRelaistest = millis();
        }
        String relaisAktiv = relaisStatus == 1 ? "1" : "0";
        lcd.setCursor(0, 1);
        lcd.print(String("Relaisstaus: " + relaisAktiv + "                   "));
        lcd.setCursor(0, 0);
        lcd.print(menuname + "                  ");
        MotorAction_StopMotors();
        MotorAction_StopBlades();
    } else if (testMenu_currentMenu == 6) {
        // MotorAction_SetPinsToGoForward();
        MotorAction_SetPinsToOnlyLeft();
        MotorAction_GoFullSpeed();
        MotorAction_StopBlades();
        RelaisOn();
        lcd.setCursor(0, 1);
        lcd.print("MOTORS ON                 ");
        lcd.setCursor(0, 0);
        lcd.print(menuname + "                  ");
    } else if (testMenu_currentMenu == 7) {
        MotorAction_StartBlades();
        RelaisOn();
        lcd.setCursor(0, 1);
        lcd.print("BLADE ON                 ");
        lcd.setCursor(0, 0);
        lcd.print(menuname + "                  ");
        MotorAction_StopMotors();
    } else {
        lcd.setCursor(0, 1);
        lcd.print("                     ");
        lcd.setCursor(0, 0);
        lcd.print(menuname + "                  ");
        MotorAction_StopMotors();
        MotorAction_StopBlades();
        RelaisOff();
    }

    read_testMenu_keys();
}
void testMenu_on_exit() {
    RelaisOff();
    MotorAction_StopMotors();
    MotorAction_StopBlades();
    RelaisOff();
    clearLCD();
}
State state_testMenu(&testMenu_on_enter, &testMenu, &testMenu_on_exit);
