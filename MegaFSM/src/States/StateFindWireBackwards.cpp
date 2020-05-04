#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <Perimeter.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/StateFindWireBackwards.h>
#include <config.h>
#include <declarations.h>


long int startTimefindWireBackwards = 0;
long int currentTimefindWireBackwards = 0;
bool wireActivefindWireBackwards = false;

void read_findWireBackwards_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(FSMEVENT_FIND_WIRE_FORWARDS__TO__PARKED, currentFSMSequence);
        return;
    }
}

void findWireBackwards_on_enter() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FIND WIRE BACKWARDS            ");
    delay(500);
    lcd.clear();
    startTimefindWireBackwards = millis();
    currentTimefindWireBackwards = startTimefindWireBackwards;
    MotorAction_SetPinsToGoForward();
    MotorAction_GoSlowSpeed();
}

void findWireBackwards() {

    wireActivefindWireBackwards = IsBounderyWireActive();
    if (wireActivefindWireBackwards == false) {
        Trigger_FSM(FSMEVENT_FIND_WIRE_FORWARDS__TO__ERROR, currentFSMSequence);
        return;
    }

    read_findWireBackwards_keys();
    lcd.setCursor(0, 0);
    lcd.print("findWireBackwards...             ");

    UpdatePerimeterStatus();

    if (MowerIsInsideWire() == false) {
        // DRIVEN OVER THE WIRE -> WIRE FOUND
        if (currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_1 || currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_2 || currentFSMSequence == FSMSEQUENCE_FOLLOW_WIRE) {
            Trigger_FSM(FSMEVENT_FIND_WIRE_FORWARDS__TO__FOLLOW_WIRE, currentFSMSequence);
            return;
        }
    }

    currentTimefindWireBackwards = millis();
    if ((currentTimefindWireBackwards - startTimefindWireBackwards) >= FIND_WIRE_SEARCH_TIME_MAX) {
        Trigger_FSM(FSMEVENT_FIND_WIRE_FORWARDS__TO__ERROR, currentFSMSequence);
        return;
    }
}

void findWireBackwards_on_exit() {
    MotorAction_StopMotors();
    MotorAction_SetPinsToGoForward();
    lcd.clear();
    startTimefindWireBackwards = 0;
    currentTimefindWireBackwards = 0;
}

State state_findWireBackwards(&findWireBackwards_on_enter, &findWireBackwards, &findWireBackwards_on_exit);
