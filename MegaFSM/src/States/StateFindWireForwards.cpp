#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <Perimeter.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/StateFindWireForwards.h>
#include <config.h>
#include <declarations.h>


long int startTimefindWireForwards = 0;
long int currentTimefindWireForwards = 0;
bool wireActivefindWireForwards = false;
long int lastTimeFindWireForwards = 0;
int wireNotFoundTotalTimefindWireForwards = 0;
long int wireNotActiveMaxTimeFindWireForwards = 500;
long int insideFalseTimeToBeOutside = 0;
long int insideFalseTimeToBeOutsideMax = 550;

void read_findWireForwards_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(BuildStateTransitionId(STATE_FIND_WIRE_FORWARDS, STATE_PARKED), currentFSMSequence);
        return;
    }
}

void findWireForwards_on_enter() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FIND WIRE FORWARDS             ");
    delay(500);
    lcd.clear();
    startTimefindWireForwards = millis();
    currentTimefindWireForwards = startTimefindWireForwards;
    lastTimeFindWireForwards = startTimefindWireForwards;
    wireNotFoundTotalTimefindWireForwards = 0;
    insideFalseTimeToBeOutside = 0;
    delay(25);
    UpdatePerimeterStatus();
    delay(25);
    MotorAction_SetPinsToGoForward();
    MotorAction_GoSlowSpeed();
}

void findWireForwards() {
    lcd.setCursor(0, 0);
    lcd.print("findWireForwards...             ");
    read_findWireForwards_keys();
    UpdatePerimeterStatus();

    lcd.setCursor(0, 1);
    lcd.print("mag: " + String(GetCurrentMagnitude()) + " i: " + String(MowerIsInsideWire()) + "           ");

    wireActivefindWireForwards = IsBounderyWireActive();
    currentTimefindWireForwards = millis();

    if (wireActivefindWireForwards == false) {
        wireNotFoundTotalTimefindWireForwards = wireNotFoundTotalTimefindWireForwards + (currentTimefindWireForwards - lastTimeFindWireForwards);
        if (wireNotFoundTotalTimefindWireForwards >= wireNotActiveMaxTimeFindWireForwards) {
            Trigger_FSM(BuildStateTransitionId(STATE_FIND_WIRE_FORWARDS, STATE_ERROR), currentFSMSequence);
            return;
        }
    } else {
        wireNotFoundTotalTimefindWireForwards = 0;
    }

    if ((currentTimefindWireForwards - startTimefindWireForwards) >= FIND_WIRE_SEARCH_TIME_MAX) {
        Trigger_FSM(BuildStateTransitionId(STATE_FIND_WIRE_FORWARDS, STATE_ERROR), currentFSMSequence);
        return;
    }

    if (MowerIsInsideWire() == false) {

        insideFalseTimeToBeOutside = insideFalseTimeToBeOutside + (currentTimefindWireForwards - lastTimeFindWireForwards);
        if (insideFalseTimeToBeOutside >= insideFalseTimeToBeOutsideMax) {
            if (currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_1 || currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_2 || currentFSMSequence == FSMSEQUENCE_FOLLOW_WIRE) {
                Trigger_FSM(BuildStateTransitionId(STATE_FIND_WIRE_FORWARDS, STATE_FOLLOW_WIRE), currentFSMSequence);
                return;
            }
        }
    } else {
        insideFalseTimeToBeOutside = 0;
    }
    lastTimeFindWireForwards = currentTimefindWireForwards;
}

void findWireForwards_on_exit() {
    MotorAction_StopMotors();
    lcd.clear();
    startTimefindWireForwards = 0;
    currentTimefindWireForwards = 0;
}

State state_findWireForwards(&findWireForwards_on_enter, &findWireForwards, &findWireForwards_on_exit);
