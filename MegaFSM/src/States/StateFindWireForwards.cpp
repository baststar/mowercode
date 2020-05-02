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


long int startTimeFindWire = 0;
long int currentTimeFindWire = 0;

void read_findWire_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(FSMEVENT_FIND_WIRE__TO__PARKED, currentFSMSequence);
    }
}

void findWire_on_enter() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FIND WIRE             ");
    delay(1000);
    lcd.clear();
    startTimeFindWire = millis();
    currentTimeFindWire = startTimeFindWire;
    MotorAction_SetPinsToGoForward();
    MotorAction_GoSlowSpeed();
    bool wireIsActive = IsBounderyWireActive();
    if (IsBounderyWireActive() == false) {
        Trigger_FSM(FSMEVENT_FIND_WIRE__TO__ERROR, currentFSMSequence);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ACTIVE? " + String(wireIsActive) + "             ");
    delay(1000);
    lcd.clear();
}

void findWire() {
    read_findWire_keys();
    lcd.setCursor(0, 0);
    lcd.print("findWire             ");

    UpdatePerimeterStatus();
    lcd.setCursor(0, 1);
    lcd.print("mag: " + String(GetCurrentMagnitude()) + "             ");

    if (MowerIsInsideWire() == false) {
        // DRIVEN OVER THE WIRE -> WIRE FOUND
        if (currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_1 || currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_2) {
            Trigger_FSM(FSMEVENT_FIND_WIRE__TO__FOLLOW_WIRE, currentFSMSequence);
        }
    }

    currentTimeFindWire = millis();
    if ((currentTimeFindWire - startTimeFindWire) >= FIND_WIRE_SEARCH_TIME_MAX) {
        Trigger_FSM(FSMEVENT_FIND_WIRE__TO__ERROR, currentFSMSequence);
    }
}

void findWire_on_exit() {
    MotorAction_StopMotors();
    MotorAction_SetPinsToGoForward();
    lcd.clear();
    startTimeFindWire = 0;
    currentTimeFindWire = 0;
}

State state_findWire(&findWire_on_enter, &findWire, &findWire_on_exit);
