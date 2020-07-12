#include <Clock.h>
#include <EEPROMVariables.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <Perimeter.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/FSMStates.h>
#include <States/StateMowing.h>
#include <VoltAmpRain.h>


unsigned long startTimeMowing = 0;
unsigned long lastTimeMowing = 0;
unsigned long insideFalseTimeToBeOutsideMowing = 0;
unsigned long insideFalseTimeToBeOutsideMowingMax = 300;
unsigned long mowTimeInMilliseconds = 0;
unsigned long currentMowTimeUntilWire = 0;
unsigned long maxMowingTimeUntilWire = 20000;

void read_mowing_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMTransition = currentFSMTransition;
        TriggerFSM(STATE_MOWING, STATE_PARKED, currentFSMSequence);
        return;
    }
}

void mowing_on_enter() {
    RelaisOn();
    startTimeMowing = millis();
    lastTimeMowing = startTimeMowing;
    insideFalseTimeToBeOutsideMowing = 0;
    currentMowTimeUntilWire = 0;

    switch (currentFSMSequence) {
    case FSMSEQUENCE_ZONE_1:
        mowTimeInMilliseconds = (unsigned long)eeprom_zone_1_mowtime * 60 * 1000;
        break;
    case FSMSEQUENCE_ZONE_2:
        mowTimeInMilliseconds = (unsigned long)eeprom_zone_2_mowtime * 60 * 1000;
        break;
    case FSMSEQUENCE_QUICK_MOW:
        mowTimeInMilliseconds = (unsigned long)eeprom_quick_mow_mowtime * 60 * 1000;
        break;
    default:
        mowTimeInMilliseconds = (unsigned long)100000;
        // ShowError("Use default1 mowtime");
        break;
    }

    MotorAction_SetPinsToGoForward();
    MotorAction_GoFullSpeed();
    MotorAction_StartBlades();
}

void mowing() {

    lcd.setCursor(0, 0);
    lcd.print("mowing                  ");
    lcd.setCursor(0, 1);
    lcd.print(GetTimeAsString());

    if (IsCharging()) {
        MotorAction_StopMotors();
        TriggerFSM(STATE_FOLLOW_WIRE, STATE_DOCKED, -1);
        ShowError("CHARGING");
        return;
    }

    if (IsRaining()) {
        MotorAction_StopMotors();
        StartSequence(FSMSEQUENCE_GO_TO_DOCK);
        return;
    }

    currentMowTimeUntilWire += (millis() - lastTimeMowing);

    if (currentMowTimeUntilWire >= maxMowingTimeUntilWire) {

        bool inside = MowerIsInsideWire();

        if (inside == false) {
            TriggerFSM(currentState, STATE_FIND_WIRE_BACKWARDS, currentFSMSequence);
            return;
        } else {
            // backwards, random rotate, findWireForwards, continue...
            MotorAction_StopMotors();
            delay(500);
            MotorAction_SetPinsToGoBackwards();
            MotorAction_GoSlowSpeed();
            delay(1500);
            TriggerFSM(currentState, STATE_RANDOM_ROTATE, currentFSMSequence);
            return;
        }
    }

    if (MowerIsInsideWire() == false) {

        insideFalseTimeToBeOutsideMowing += (millis() - lastTimeMowing);

        if (insideFalseTimeToBeOutsideMowing >= insideFalseTimeToBeOutsideMowingMax) {

            MotorAction_StopMotors();
            delay(500);
            MotorAction_SetPinsToGoBackwards();
            MotorAction_GoSlowSpeed();
            delay(1000);

            if (currentFSMSequence == FSMSEQUENCE_ZONE_1 || currentFSMSequence == FSMSEQUENCE_ZONE_2 || currentFSMSequence == FSMSEQUENCE_QUICK_MOW) {
                TriggerFSM(STATE_MOWING, STATE_RANDOM_ROTATE, currentFSMSequence);
                return;
            } else {
                TriggerFSM(STATE_MOWING, STATE_ERROR, currentFSMSequence);
                return;
            }
        }

    } else {
        insideFalseTimeToBeOutsideMowing = 0;
    }

    if ((sequenceStartTime + mowTimeInMilliseconds) < millis()) {
        MotorAction_StopMotors();
        ShowError("MOWING COMPLETE");
        StartSequence(FSMSEQUENCE_GO_TO_DOCK);
        return;
    }

    lastTimeMowing = millis();
    read_mowing_keys();
}

void mowing_on_exit() {
    MotorAction_StopBlades();
    MotorAction_StopMotors();
    clearLCD();
}

State state_mowing(&mowing_on_enter, &mowing, &mowing_on_exit);
