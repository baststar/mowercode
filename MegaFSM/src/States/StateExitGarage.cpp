#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/StateExitGarage.h>
#include <config.h>


long int startTimeExitGarage = 0;
long int currentTimeExitGarage = 0;

void read_exitGarage_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(FSMEVENT_EXIT_GARAGE__TO__PARKED, currentFSMSequence);
    }
}

void exitGarage_on_enter() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("EXIT GARAGE                ");
    delay(1000);
    lcd.clear();

    startTimeExitGarage = millis();
    currentTimeExitGarage = startTimeExitGarage;
    MotorAction_SetPinsToGoBackwards();
    MotorAction_GoSlowSpeed();
}

void exitGarage() {
    read_exitGarage_keys();
    lcd.setCursor(0, 0);
    lcd.print("exitGarage             ");

    currentTimeExitGarage = millis();
    if ((currentTimeExitGarage - startTimeExitGarage) >= EXIT_GARAGE_BACKWARDS_TIME) {
        if (currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_1 || currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_2) {
            Trigger_FSM(FSMEVENT_EXIT_GARAGE__TO__ROTATE_TO_WIRE, currentFSMSequence);
        } else if (currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE__RANDOM_ROTATE__MOWING) {
            Trigger_FSM(FSMEVENT_EXIT_GARAGE__TO__RANDOM_ROTATE, currentFSMSequence);
        } else {
            Trigger_FSM(FSMEVENT_EXIT_GARAGE__TO__ERROR, currentFSMSequence);
        }
    }
}

void exitGarage_on_exit() {
    lcd.clear();
    MotorAction_StopMotors();
    MotorAction_SetPinsToGoForward();
}
State state_exitGarage(&exitGarage_on_enter, &exitGarage, &exitGarage_on_exit);
