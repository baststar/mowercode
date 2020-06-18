#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <States/FSMMower.h>
#include <States/FSMStates.h>
#include <States/StateCompassRotate.h>


void read_compassRotate_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMTransition = currentFSMTransition;
        TriggerFSM(STATE_COMPASS_ROTATE, STATE_PARKED, currentFSMSequence);
        return;
    }
}

void compassRotate_on_enter() {
    RelaisOn();
}

void compassRotate() {
    lcd.setCursor(0, 0);
    lcd.print("compassRotate                   ");
    read_compassRotate_keys();
}

void compassRotate_on_exit() {
    clearLCD();
}
State state_compassRotate(&compassRotate_on_enter, &compassRotate, &compassRotate_on_exit);
