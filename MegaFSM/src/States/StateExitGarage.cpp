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
        int stateId = (String(STATE_EXIT_GARAGE) + 9999 + String(STATE_PARKED)).toInt();
        Trigger_FSM(stateId, currentFSMSequence);
        return;
    }
}

void exitGarage_on_enter() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("EXIT GARAGE                ");
    delay(500);
    lcd.clear();

    startTimeExitGarage = millis();
    currentTimeExitGarage = startTimeExitGarage;
    MotorAction_SetPinsToGoBackwards();
    MotorAction_GoFullSpeed();
}

void exitGarage() {
    read_exitGarage_keys();
    lcd.setCursor(0, 0);
    lcd.print("exitGarage...             ");

    currentTimeExitGarage = millis();
    if ((currentTimeExitGarage - startTimeExitGarage) >= EXIT_GARAGE_BACKWARDS_TIME) {
        if (currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_1 || currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_2) {
            int stateId = (String(STATE_EXIT_GARAGE) + String(9999) + String(STATE_ROTATE_TO_WIRE)).toInt();
            Trigger_FSM(stateId, currentFSMSequence);
            return;
        } else if (currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE__RANDOM_ROTATE__MOWING) {
            int stateId = (String(STATE_EXIT_GARAGE) + String(9999) + String(STATE_RANDOM_ROTATE)).toInt();
            Trigger_FSM(stateId, currentFSMSequence);
            return;
        } else {
            int stateId = (String(STATE_EXIT_GARAGE) + String(9999) + String(STATE_ERROR)).toInt();
            Trigger_FSM(stateId, currentFSMSequence);
            return;
        }
    }
}

void exitGarage_on_exit() {
    MotorAction_StopMotors();
    MotorAction_SetPinsToGoForward();
    lcd.clear();
}
State state_exitGarage(&exitGarage_on_enter, &exitGarage, &exitGarage_on_exit);
