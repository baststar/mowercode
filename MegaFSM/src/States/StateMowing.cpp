#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <Perimeter.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/StateMowing.h>


long int startTimeMowing = 0;
long int currentTimeMowing = 0;
long int lastTimeMowing = 0;
long int insideFalseTimeToBeOutsideMowing = 0;
long int insideFalseTimeToBeOutsideMowingMax = 300;

void read_mowing_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(BuildStateTransitionId(STATE_MOWING, STATE_PARKED), currentFSMSequence);
        return;
    }
}

void mowing_on_enter() {
    clearLCD();
    lcd.setCursor(0, 0);
    lcd.print("MOWING                    ");
    delay(500);
    clearLCD();
    startTimeMowing = millis();
    currentTimeMowing = startTimeMowing;
    lastTimeMowing = startTimeMowing;
    insideFalseTimeToBeOutsideMowing = 0;
    MotorAction_SetPinsToGoForward();
    MotorAction_GoFullSpeed();
    MotorAction_StartBlades();
}

void mowing() {
    read_mowing_keys();
    lcd.setCursor(0, 0);
    lcd.print("mowing                  ");

    UpdatePerimeterStatus();

    if (MowerIsInsideWire() == false) {

        currentTimeMowing = millis();

        insideFalseTimeToBeOutsideMowing = insideFalseTimeToBeOutsideMowing + (currentTimeMowing - lastTimeMowing);

        if (insideFalseTimeToBeOutsideMowing >= insideFalseTimeToBeOutsideMowingMax) {

            MotorAction_StopMotors();
            delay(500);
            MotorAction_SetPinsToGoBackwards();
            MotorAction_GoSlowSpeed();
            delay(1000);

            if (currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_1 || currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_2 ||
                currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE__RANDOM_ROTATE__MOWING) {
                Trigger_FSM(BuildStateTransitionId(STATE_MOWING, STATE_RANDOM_ROTATE), currentFSMSequence);
                return;
            } else {
                Trigger_FSM(BuildStateTransitionId(STATE_MOWING, STATE_ERROR), currentFSMSequence);
                return;
            }
        }

    } else {
        insideFalseTimeToBeOutsideMowing = 0;
    }
}

void mowing_on_exit() {
    MotorAction_StopBlades();
    MotorAction_StopMotors();
    clearLCD();
}

State state_mowing(&mowing_on_enter, &mowing, &mowing_on_exit);
