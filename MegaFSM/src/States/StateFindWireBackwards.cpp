#include <EEPROMVariables.h>
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
        delay(100);
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(BuildStateTransitionId(STATE_FIND_WIRE_BACKWARDS, STATE_PARKED), currentFSMSequence);
        return;
    }
}

void findWireBackwards_on_enter() {
    clearLCD();
    lcd.setCursor(0, 0);
    lcd.print("FIND WIRE BACKWARDS            ");
    delay(500);
    clearLCD();
    startTimefindWireBackwards = millis();
    currentTimefindWireBackwards = startTimefindWireBackwards;
    MotorAction_SetPinsToGoForward();
    MotorAction_GoSlowSpeed();
}

void findWireBackwards() {

    wireActivefindWireBackwards = IsBounderyWireActive();
    if (wireActivefindWireBackwards == false) {
        Trigger_FSM(BuildStateTransitionId(STATE_FIND_WIRE_BACKWARDS, STATE_ERROR), currentFSMSequence);
        return;
    }

    read_findWireBackwards_keys();
    lcd.setCursor(0, 0);
    lcd.print("findWireBackwards             ");

    UpdatePerimeterStatus();

    if (MowerIsInsideWire() == false) {
        // DRIVEN OVER THE WIRE -> WIRE FOUND
        if (currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_1 || currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_2 || currentFSMSequence == FSMSEQUENCE_FOLLOW_WIRE) {
            Trigger_FSM(BuildStateTransitionId(STATE_FIND_WIRE_BACKWARDS, STATE_FOLLOW_WIRE), currentFSMSequence);
            return;
        }
    }

    currentTimefindWireBackwards = millis();
    if ((currentTimefindWireBackwards - startTimefindWireBackwards) >= eeprom_find_wire_search_time_max) {
        Trigger_FSM(BuildStateTransitionId(STATE_FIND_WIRE_BACKWARDS, STATE_ERROR), currentFSMSequence);
        return;
    }
}

void findWireBackwards_on_exit() {
    MotorAction_StopMotors();
    MotorAction_SetPinsToGoForward();
    clearLCD();
    startTimefindWireBackwards = 0;
    currentTimefindWireBackwards = 0;
}

State state_findWireBackwards(&findWireBackwards_on_enter, &findWireBackwards, &findWireBackwards_on_exit);
