#include <Clock.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/StateDocked.h>

void read_docked_keys() {
    Read_Membrane_Keys();
    if (StartKey_pressed == 0) {
        delay(250);
        Trigger_FSM(BuildStateTransitionId(STATE_DOCKED, STATE_DOCKED_MENU), -1);
        return;
    }
}

void docked_on_enter() {
    MotorAction_StopBlades();
    MotorAction_StopMotors();
    clearLCD();
    lcd.setCursor(0, 0);
    lcd.print("DOCKED             ");
    delay(500);
    clearLCD();
    ResetScrollRow0Text();
}

void docked() {
    read_docked_keys();
    // ScrollRow0Text("docked...", "row2 text");
    lcd.setCursor(0, 0);
    lcd.print("docked                ");
}

void docked_on_exit() {
    clearLCD();
}

State state_docked(&docked_on_enter, &docked, &docked_on_exit);
