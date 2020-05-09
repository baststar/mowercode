#include <EEPROMVariables.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/StateRandomRotate.h>
#include <config.h>


long int currentRotationTime = 0;
long int maxRotationTime = 0;
long int lastRotation = 0;

void read_randomRotate_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMEvent = currentFSMEvent;
        TriggerFSM(STATE_RANDOM_ROTATE, STATE_PARKED, currentFSMSequence);
        return;
    }
    lastRotation = millis();
}

void randomRotate_on_enter() {
    clearLCD();
    lcd.setCursor(0, 0);
    lcd.print("RANDOM ROTATE                ");
    delay(500);
    clearLCD();
    currentRotationTime = 0;
    maxRotationTime = random(eeprom_random_rotate_time_min, eeprom_random_rotate_time_max);
    if (eeprom_perimeter_is_clockwise_from_garage == 1) {
        MotorAction_SetPinsToRotateLeft();
    } else {
        MotorAction_SetPinsToRotateRight();
    }
    MotorAction_GoSlowSpeed();
}
void randomRotate() {

    read_randomRotate_keys();
    lcd.setCursor(0, 0);
    lcd.print("randomRotate                ");

    currentRotationTime = currentRotationTime + (millis() - lastRotation);

    if (currentRotationTime >= maxRotationTime) {
        if (currentFSMSequence == FSMSEQUENCE_ZONE_1 || currentFSMSequence == FSMSEQUENCE_ZONE_2 || currentFSMSequence == FSMSEQUENCE_QUICK_MOW) {
            TriggerFSM(STATE_RANDOM_ROTATE, STATE_MOWING, currentFSMSequence);
            return;
        } else {
            TriggerFSM(STATE_RANDOM_ROTATE, STATE_ERROR, currentFSMSequence);
            return;
        }
    }

    lastRotation = millis();
}
void randomRotate_on_exit() {
    MotorAction_StopMotors();
    clearLCD();
}

State state_randomRotate(&randomRotate_on_enter, &randomRotate, &randomRotate_on_exit);
