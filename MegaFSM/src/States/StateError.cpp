#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/StateParked.h>


void read_error_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(FSMEVENT_ERROR__TO__PARKED, currentFSMSequence);
    }
}

void error_on_enter() {
}

void error() {
    read_error_keys();
    lcd.setCursor(0, 0);
    lcd.print("ERROR...                ");
    lcd.setCursor(0, 1);

    if (StopKey_pressed == 0) {
        Trigger_FSM(FSMEVENT_ERROR__TO__PARKED, -1);
    }

    switch (lastFSMEvent) {
    case FSMEVENT_EXIT_GARAGE__TO__FIND_WIRE_FORWARDS:
    case FSMEVENT_ROTATE_TO_WIRE__TO__FIND_WIRE_FORWARDS:
    case FSMEVENT_MOWING__TO__FIND_WIRE_FORWARDS:
    case FSMEVENT_PARKED_MENU__TO__FIND_WIRE_FORWARDS:
        lcd.print("FIND WIRE FORWARDS                     ");
        break;
    case FSMEVENT_EXIT_GARAGE__TO__FIND_WIRE_BACKWARDS:
    case FSMEVENT_ROTATE_TO_WIRE__TO__FIND_WIRE_BACKWARDS:
    case FSMEVENT_MOWING__TO__FIND_WIRE_BACKWARDS:
    case FSMEVENT_PARKED_MENU__TO__FIND_WIRE_BACKWARDS:
        lcd.print("FIND WIRE BACKWARDS                    ");
        break;
    default:
        lcd.print("UNKNOWN ERR " + String(lastFSMEvent) + "               ");
        break;
    }
}

void error_on_exit() {
    lcd.clear();
}

State state_error(&error_on_enter, &error, &error_on_exit);
