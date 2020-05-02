#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/StateFollowWire.h>
#include <config.h>


long int startTimeFollowWire = 0;
long int currentTimeFollowWire = 0;
bool dockedFollowWire = false;

void read_followWire_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(FSMEVENT_FOLLOW_WIRE__TO__PARKED, currentFSMSequence);
    }
}

void followWire_on_enter() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FOLLOW WIRE             ");
    delay(1000);
    lcd.clear();
    startTimeFollowWire = millis();
    currentTimeFollowWire = startTimeFollowWire;
}

void followWire() {
    read_followWire_keys();
    lcd.setCursor(0, 0);
    lcd.print("followWire                ");

    currentTimeFollowWire = millis();

    if (currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_1) {
        if ((currentTimeFollowWire - startTimeFollowWire) >= FOLLOW_WIRE_ZONE_1_TIME) {
            Trigger_FSM(FSMEVENT_FOLLOW_WIRE__TO__WIRE_TO_GARDEN, currentFSMSequence);
        }
    } else if (currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_2) {
        if ((currentTimeFollowWire - startTimeFollowWire) >= FOLLOW_WIRE_ZONE_2_TIME) {
            Trigger_FSM(FSMEVENT_FOLLOW_WIRE__TO__WIRE_TO_GARDEN, currentFSMSequence);
        }
    }

    if (dockedFollowWire) {
        Trigger_FSM(FSMEVENT_FOLLOW_WIRE__TO__DOCKED, -1);
    }
}

void followWire_on_exit() {
    lcd.clear();
    startTimeFollowWire = 0;
    currentTimeFollowWire = 0;
}

State state_followWire(&followWire_on_enter, &followWire, &followWire_on_exit);
