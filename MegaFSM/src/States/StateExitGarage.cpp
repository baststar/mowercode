#include <EEPROMVariables.h>
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
        delay(250);
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(BuildStateTransitionId(STATE_EXIT_GARAGE, STATE_PARKED), currentFSMSequence);
        return;
    }
}

void exitGarage_on_enter() {
    clearLCD();
    lcd.setCursor(0, 0);
    lcd.print("EXIT GARAGE                ");
    delay(500);
    clearLCD();

    startTimeExitGarage = millis();
    currentTimeExitGarage = startTimeExitGarage;
    MotorAction_SetPinsToGoBackwards();
    MotorAction_GoFullSpeed();
}

void exitGarage() {
    read_exitGarage_keys();
    lcd.setCursor(0, 0);
    lcd.print("exitGarage             ");

    currentTimeExitGarage = millis();
    if ((currentTimeExitGarage - startTimeExitGarage) >= eeprom_exit_garage_backwards_time) {
        if (currentFSMSequence == FSMSEQUENCE_ZONE_1 || currentFSMSequence == FSMSEQUENCE_ZONE_2) {
            Trigger_FSM(BuildStateTransitionId(STATE_EXIT_GARAGE, STATE_ROTATE_TO_WIRE), currentFSMSequence);
            return;
        } else if (currentFSMSequence == FSMSEQUENCE_QUICK_MOW) {
            Trigger_FSM(BuildStateTransitionId(STATE_EXIT_GARAGE, STATE_RANDOM_ROTATE), currentFSMSequence);
            return;
        } else {
            Trigger_FSM(BuildStateTransitionId(STATE_EXIT_GARAGE, STATE_ERROR), currentFSMSequence);
            return;
        }
    }
}

void exitGarage_on_exit() {
    MotorAction_StopMotors();
    MotorAction_SetPinsToGoForward();
    clearLCD();
}
State state_exitGarage(&exitGarage_on_enter, &exitGarage, &exitGarage_on_exit);
