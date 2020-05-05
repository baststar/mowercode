#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/StateParked.h>


void read_parked_keys() {
    Read_Membrane_Keys();
    if (StartKey_pressed == 0) {
        Trigger_FSM(BuildStateTransitionId(STATE_PARKED, STATE_PARKED_MENU), currentFSMSequence);
        return;
    }
}

void parked_on_enter() {
    MotorAction_StopMotors();
    MotorAction_StopBlades();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PARKED                     ");
    delay(500);
    lcd.clear();
}

void parked() {
    read_parked_keys();
    lcd.setCursor(0, 0);
    lcd.print("parked...                  ");
}

void parked_on_exit() {
    lcd.clear();
}

State state_parked(&parked_on_enter, &parked, &parked_on_exit);
