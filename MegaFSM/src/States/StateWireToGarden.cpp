#include <EEPROMVariables.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/FSMStates.h>
#include <States/StateWireToGarden.h>
#include <config.h>

unsigned long startTimeWireToGarden = 0;
unsigned long currentTimeWireToGarden = 0;
unsigned long lastTimeCheckedWireToGardenRotation = 0;

void read_wireToGarden_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMTransition = currentFSMTransition;
        TriggerFSM(STATE_WIRE_TO_GARDEN, STATE_PARKED, currentFSMSequence);
        return;
    }
}

void wireToGarden_on_enter() {
    startTimeWireToGarden = millis();
    lastTimeCheckedWireToGardenRotation = startTimeWireToGarden;
    MotorAction_GoSlowSpeed();
}

void wireToGarden() {

    lcd.setCursor(0, 0);
    lcd.print("wireToGarden                     ");

    if (currentMillisGlobal - startTimeWireToGarden < eeprom_rotate_from_wire_to_garden_time) {
        if (eeprom_perimeter_is_clockwise_from_garage == 1) {
            MotorAction_SetPinsToRotateLeft();
        } else {
            MotorAction_SetPinsToRotateRight();
        }
    } else {
        MotorAction_SetPinsToGoForward();
    }

    if ((currentMillisGlobal - startTimeWireToGarden) >= (eeprom_wire_into_garden_time + eeprom_rotate_from_wire_to_garden_time)) {
        TriggerFSM(STATE_WIRE_TO_GARDEN, STATE_MOWING, currentFSMSequence);
        return;
    }

    lastTimeCheckedWireToGardenRotation = currentMillisGlobal;

    read_wireToGarden_keys();
}

void wireToGarden_on_exit() {
    MotorAction_StopMotors();
    clearLCD();
}

State state_wireToGarden(&wireToGarden_on_enter, &wireToGarden, &wireToGarden_on_exit);
