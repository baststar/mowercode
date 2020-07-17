#include <Clock.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <Perimeter.h>
#include <States/FSMMower.h>
#include <States/FSMStates.h>
#include <States/StateDocked.h>
#include <VoltAmpRain.h>


void read_docked_keys() {
    Read_Membrane_Keys();
    if (StartKey_pressed == 0) {
        delay(250);
        clearLCD();
        TriggerFSM(STATE_DOCKED, STATE_DOCKED_MENU, -1);
        return;
    }
}

void docked_on_enter() {
    RelaisOff();
    MotorAction_StopBlades();
    MotorAction_StopMotors();
}

void docked() {

    lcd.setCursor(0, 0);
    lcd.print("docked " + String(GetBatteryVolt()) + "            ");
    if (IsCharging()) {
        lcd.setCursor(0, 1);
        lcd.print(GetTimeAsString() + " charging  ");
    }
    // else if (!IsBounderyWireActive()) {
    //     lcd.setCursor(0, 1);
    //     lcd.print(GetTimeAsString() + " no wire   ");
    // }
    else {
        lcd.setCursor(0, 1);
        lcd.print(GetTimeAsString() + "           ");
    }
    CheckAlerts();
    read_docked_keys();
}

void docked_on_exit() {
    clearLCD();
}

State state_docked(&docked_on_enter, &docked, &docked_on_exit);
