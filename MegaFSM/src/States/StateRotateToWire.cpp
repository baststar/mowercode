#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/StateRotateToWire.h>
#include <config.h>



long int startTimeRotateToWire = 0;
long int currentRotateToWireTime = 0;

void read_rotateToWire_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(BuildStateTransitionId(STATE_ROTATE_TO_WIRE, STATE_PARKED), currentFSMSequence);
        return;
    }
}

void rotateToWire_on_enter() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ROTATE TO WIRE                ");
    delay(500);
    lcd.clear();
    startTimeRotateToWire = millis();
    currentRotateToWireTime = startTimeRotateToWire;

    if (PERIMETER_IS_CLOCKWISE_FROM_GARAGE) {
        MotorAction_SetPinsToRotateLeft();
    } else {
        MotorAction_SetPinsToRotateRight();
    }
    MotorAction_GoSlowSpeed();
}
void rotateToWire() {

    read_rotateToWire_keys();
    lcd.setCursor(0, 0);
    lcd.print("rotateToWire...                ");

    currentRotateToWireTime = millis();

    if ((currentRotateToWireTime - startTimeRotateToWire) >= ROTATE_FROM_EXIT_GARAGE_TO_WIRE_TIME) {
        if (currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_1 || currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_2) {
            Trigger_FSM(BuildStateTransitionId(STATE_ROTATE_TO_WIRE, STATE_FIND_WIRE_FORWARDS), currentFSMSequence);
            return;
        } else if (currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE__RANDOM_ROTATE__MOWING) {
            Trigger_FSM(BuildStateTransitionId(STATE_ROTATE_TO_WIRE, STATE_RANDOM_ROTATE), currentFSMSequence);
            return;
        } else {
            Trigger_FSM(BuildStateTransitionId(STATE_ROTATE_TO_WIRE, STATE_ERROR), currentFSMSequence);
            return;
        }
    }
}
void rotateToWire_on_exit() {
    MotorAction_StopMotors();
    MotorAction_SetPinsToGoForward();
    lcd.clear();
}

State state_rotateToWire(&rotateToWire_on_enter, &rotateToWire, &rotateToWire_on_exit);
