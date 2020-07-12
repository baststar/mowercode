#include <EEPROMVariables.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <Perimeter.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/FSMStates.h>
#include <States/StateExitGarage.h>
#include <VoltAmpRain.h>
#include <config.h>



unsigned long startTimeExitGarage = 0;

void read_exitGarage_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMTransition = currentFSMTransition;
        TriggerFSM(STATE_EXIT_GARAGE, STATE_PARKED, currentFSMSequence);
        return;
    }
}

void exitGarage_on_enter() {
    RelaisOn();
    startTimeExitGarage = millis();
    MotorAction_SetPinsToGoBackwards();
    MotorAction_GoFullSpeed();
}

void exitGarage() {
    lcd.setCursor(0, 0);
    lcd.print("exitGarage             ");

    if ((currentMillisGlobal - startTimeExitGarage) >= eeprom_exit_garage_backwards_time) {
        if (currentFSMSequence == FSMSEQUENCE_ZONE_1 || currentFSMSequence == FSMSEQUENCE_ZONE_2) {
            TriggerFSM(STATE_EXIT_GARAGE, STATE_ROTATE_TO_WIRE, currentFSMSequence);
            return;
        } else if (currentFSMSequence == FSMSEQUENCE_QUICK_MOW) {
            TriggerFSM(STATE_EXIT_GARAGE, STATE_RANDOM_ROTATE, currentFSMSequence);
            return;
        } else {
            TriggerFSM(STATE_EXIT_GARAGE, STATE_ERROR, currentFSMSequence);
            return;
        }
    }
    read_exitGarage_keys();
}

void exitGarage_on_exit() {
    MotorAction_StopMotors();
    MotorAction_SetPinsToGoForward();
    clearLCD();
}
State state_exitGarage(&exitGarage_on_enter, &exitGarage, &exitGarage_on_exit);
