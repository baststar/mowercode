#include <EEPROMVariables.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <Perimeter.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/FSMStates.h>
#include <States/StateFindWireBackwards.h>
#include <config.h>



unsigned long startTimefindWireBackwards = 0;
bool wireActivefindWireBackwards = false;
unsigned long insideTrueTimeToBeInside = 0;
unsigned long lastTimeFindWireBackwards = 0;
unsigned long insideTrueTimeToBeInsideMax = 200;

void read_findWireBackwards_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMTransition = currentFSMTransition;
        TriggerFSM(STATE_FIND_WIRE_BACKWARDS, STATE_PARKED, currentFSMSequence);
        return;
    }
}

void findWireBackwards_on_enter() {
    startTimefindWireBackwards = millis();
    insideTrueTimeToBeInside = 0;
    MotorAction_SetPinsToGoBackwards();
    MotorAction_GoSlowSpeed();
}

void findWireBackwards() {

    lcd.setCursor(0, 0);
    lcd.print("findWireBackwards             ");

    if ((millis() - startTimefindWireBackwards) >= eeprom_find_wire_search_time_max) {
        TriggerFSM(STATE_FIND_WIRE_BACKWARDS, STATE_ERROR, currentFSMSequence);
        ShowError("max find wire time");
        return;
    }

    if (MowerIsInsideWire() == true) {

        insideTrueTimeToBeInside += (millis() - lastTimeFindWireBackwards);

        if (insideTrueTimeToBeInside >= insideTrueTimeToBeInsideMax) {
            MotorAction_StopMotors();
        }

        if (currentFSMSequence == FSMSEQUENCE_ZONE_1 || currentFSMSequence == FSMSEQUENCE_ZONE_2 || currentFSMSequence == FSMSEQUENCE_QUICK_MOW || currentFSMSequence == FSMSEQUENCE_GO_TO_DOCK) {
            TriggerFSM(currentState, STATE_FOLLOW_WIRE, currentFSMSequence);
            return;
        } else {
            ShowError("wrong seq " + String(currentFSMSequence));
            TriggerFSM(currentState, STATE_ERROR, currentFSMSequence);
            return;
        }

    } else {
        insideTrueTimeToBeInside = 0;
    }

    lastTimeFindWireBackwards = millis();
    read_findWireBackwards_keys();
}

void findWireBackwards_on_exit() {
    MotorAction_StopMotors();
    clearLCD();
}

State state_findWireBackwards(&findWireBackwards_on_enter, &findWireBackwards, &findWireBackwards_on_exit);
