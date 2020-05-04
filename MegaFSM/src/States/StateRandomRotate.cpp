#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/StateRandomRotate.h>
#include <config.h>



long int currentRotationTime = 0;
long int maxRotationTime = 0;
long int lastRotation = 0;

void read_randomRotate_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(FSMEVENT_RANDOM_ROTATE__TO__PARKED, currentFSMSequence);
        return;
    }
    lastRotation = millis();
}

void randomRotate_on_enter() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RANDOM ROTATE                ");
    delay(500);
    lcd.clear();
    currentRotationTime = 0;
    maxRotationTime = random(RANDOM_ROTATE_TIME_MIN, RANDOM_ROTATE_TIME_MAX);
    if (PERIMETER_IS_CLOCKWISE_FROM_GARAGE == true) {
        MotorAction_SetPinsToRotateLeft();
    } else {
        MotorAction_SetPinsToRotateRight();
    }
    MotorAction_GoSlowSpeed();
}
void randomRotate() {

    read_randomRotate_keys();
    lcd.setCursor(0, 0);
    lcd.print("randomRotate...                ");

    currentRotationTime = currentRotationTime + (millis() - lastRotation);

    if (currentRotationTime >= maxRotationTime) {
        if (currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_1 || currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_2 ||
            currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE__RANDOM_ROTATE__MOWING) {
            Trigger_FSM(FSMEVENT_RANDOM_ROTATE__TO__MOWING, currentFSMSequence);
            return;
        } else {
            Trigger_FSM(FSMEVENT_RANDOM_ROTATE__TO__ERROR, currentFSMSequence);
            return;
        }
    }

    lastRotation = millis();
}
void randomRotate_on_exit() {
    MotorAction_StopMotors();
    lcd.clear();
}

State state_randomRotate(&randomRotate_on_enter, &randomRotate, &randomRotate_on_exit);
