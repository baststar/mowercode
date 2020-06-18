#include <EEPROMVariables.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <Perimeter.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/FSMStates.h>
#include <States/StateFindWireForwards.h>
#include <VoltAmpRain.h>
#include <config.h>


unsigned long startTimefindWireForwards = 0;
unsigned long currentTimefindWireForwards = 0;
unsigned long lastTimeFindWireForwards = 0;
unsigned long insideFalseTimeToBeOutside = 0;
unsigned long insideFalseTimeToBeOutsideMax = 200;

void read_findWireForwards_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMTransition = currentFSMTransition;
        TriggerFSM(STATE_FIND_WIRE_FORWARDS, STATE_PARKED, currentFSMSequence);
        return;
    }
}

void findWireForwards_on_enter() {
    startTimefindWireForwards = millis();
    insideFalseTimeToBeOutside = 0;
    MotorAction_SetPinsToGoForward();
    MotorAction_GoSlowSpeed();
}

void findWireForwards() {

    lcd.setCursor(0, 0);
    lcd.print("findWireF " + String(MowerIsInsideWire()));

    if ((millis() - startTimefindWireForwards) >= eeprom_find_wire_search_time_max) {
        TriggerFSM(STATE_FIND_WIRE_FORWARDS, STATE_ERROR, currentFSMSequence);
        ShowError("max find wire time");
        return;
    }

    if (MowerIsInsideWire() == false) {

        insideFalseTimeToBeOutside += (millis() - lastTimeFindWireForwards);

        if (insideFalseTimeToBeOutside >= insideFalseTimeToBeOutsideMax) {

            MotorAction_StopMotors();
            delay(500);
            MotorAction_SetPinsToGoBackwards();
            MotorAction_GoSlowSpeed();
            delay(500);

            if (currentFSMSequence == FSMSEQUENCE_ZONE_1 || currentFSMSequence == FSMSEQUENCE_ZONE_2 || currentFSMSequence == FSMSEQUENCE_QUICK_MOW || currentFSMSequence == FSMSEQUENCE_GO_TO_DOCK) {
                TriggerFSM(currentState, STATE_FOLLOW_WIRE, currentFSMSequence);
                return;
            } else {
                ShowError("wrong seq " + String(currentFSMSequence));
                TriggerFSM(currentState, STATE_ERROR, currentFSMSequence);
                return;
            }
        }

    } else {
        insideFalseTimeToBeOutside = 0;
    }

    lastTimeFindWireForwards = millis();
    read_findWireForwards_keys();
}

void findWireForwards_on_exit() {
    MotorAction_StopMotors();
    clearLCD();
}

State state_findWireForwards(&findWireForwards_on_enter, &findWireForwards, &findWireForwards_on_exit);
