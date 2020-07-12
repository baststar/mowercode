#include <EEPROMVariables.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/FSMStates.h>
#include <States/StateRandomRotate.h>
#include <config.h>

unsigned long maxRotationTime = 0;
unsigned long startRotationTime = 0;

void read_randomRotate_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMTransition = currentFSMTransition;
        TriggerFSM(STATE_RANDOM_ROTATE, STATE_PARKED, currentFSMSequence);
        return;
    }
}

void randomRotate_on_enter() {
    RelaisOn();
    if (eeprom_perimeter_is_clockwise_from_garage == 1) {
        MotorAction_SetPinsToRotateLeft();
    } else {
        MotorAction_SetPinsToRotateRight();
    }
    MotorAction_GoSlowSpeed();
    maxRotationTime = random(eeprom_random_rotate_time_min, eeprom_random_rotate_time_max);
    startRotationTime = millis();
}

void randomRotate() {

    lcd.setCursor(0, 0);
    lcd.print("randomRotate                ");

    if ((millis() - startRotationTime) > maxRotationTime) {
        if (currentFSMSequence == FSMSEQUENCE_ZONE_1 || currentFSMSequence == FSMSEQUENCE_ZONE_2 || currentFSMSequence == FSMSEQUENCE_QUICK_MOW) {
            TriggerFSM(STATE_RANDOM_ROTATE, STATE_MOWING, currentFSMSequence);
            return;
        } else if (currentFSMSequence == FSMSEQUENCE_GO_TO_DOCK) {
            TriggerFSM(currentState, STATE_FIND_WIRE_FORWARDS, currentFSMSequence);
            return;
        } else {
            TriggerFSM(STATE_RANDOM_ROTATE, STATE_ERROR, currentFSMSequence);
            return;
        }
    }
    read_randomRotate_keys();
}

void randomRotate_on_exit() {
    MotorAction_StopMotors();
    clearLCD();
}

State state_randomRotate(&randomRotate_on_enter, &randomRotate, &randomRotate_on_exit);
