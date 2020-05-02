#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/StateRandomRotate.h>
#include <config.h>


long int currentRotationTime = 0;
long int maxRotationTime = 0;

void read_randomRotate_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(FSMEVENT_RANDOM_ROTATE__TO__PARKED, currentFSMSequence);
    }
}

void randomRotate_on_enter() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RANDOM ROTATE                ");
    delay(1000);
    lcd.clear();
    currentRotationTime = 0;
    maxRotationTime = random(RANDOM_ROTATE_TIME_MIN, RANDOM_ROTATE_TIME_MAX);
}
void randomRotate() {

    read_randomRotate_keys();
    lcd.setCursor(0, 0);
    lcd.print("randomRotate                ");

    if (currentRotationTime >= maxRotationTime) {
        if (currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE__RANDOM_ROTATE__MOWING) {
            Trigger_FSM(FSMEVENT_RANDOM_ROTATE__TO__MOWING, currentFSMSequence);
        } else {
            Trigger_FSM(FSMEVENT_RANDOM_ROTATE__TO__ERROR, currentFSMSequence);
        }
    }
}
void randomRotate_on_exit() {
    lcd.clear();
}

State state_randomRotate(&randomRotate_on_enter, &randomRotate, &randomRotate_on_exit);
