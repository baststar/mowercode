#include <EEPROMVariables.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/StateRotateToWire.h>
#include <config.h>


long int startTimeRotateToWire = 0;
long int currentRotateToWireTime = 0;

void read_rotateToWire_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMEvent = currentFSMEvent;
        TriggerFSM(STATE_ROTATE_TO_WIRE, STATE_PARKED, currentFSMSequence);
        return;
    }
}

void rotateToWire_on_enter() {
    clearLCD();
    lcd.setCursor(0, 0);
    lcd.print("ROTATE TO WIRE                ");
    delay(500);
    clearLCD();
    startTimeRotateToWire = millis();
    currentRotateToWireTime = startTimeRotateToWire;

    if (eeprom_perimeter_is_clockwise_from_garage == 1) {
        MotorAction_SetPinsToRotateLeft();
    } else {
        MotorAction_SetPinsToRotateRight();
    }
    MotorAction_GoSlowSpeed();
}
void rotateToWire() {

    read_rotateToWire_keys();
    lcd.setCursor(0, 0);
    lcd.print("rotateToWire                ");

    currentRotateToWireTime = millis();

    if ((currentRotateToWireTime - startTimeRotateToWire) >= eeprom_rotate_from_exit_garage_to_wire_time) {
        if (currentFSMSequence == FSMSEQUENCE_ZONE_1 || currentFSMSequence == FSMSEQUENCE_ZONE_2) {
            TriggerFSM(STATE_ROTATE_TO_WIRE, STATE_FIND_WIRE_FORWARDS, currentFSMSequence);
            return;
        } else if (currentFSMSequence == FSMSEQUENCE_QUICK_MOW) {
            TriggerFSM(STATE_ROTATE_TO_WIRE, STATE_RANDOM_ROTATE, currentFSMSequence);
            return;
        } else {
            TriggerFSM(STATE_ROTATE_TO_WIRE, STATE_ERROR, currentFSMSequence);
            return;
        }
    }
}
void rotateToWire_on_exit() {
    MotorAction_StopMotors();
    MotorAction_SetPinsToGoForward();
    clearLCD();
}

State state_rotateToWire(&rotateToWire_on_enter, &rotateToWire, &rotateToWire_on_exit);
