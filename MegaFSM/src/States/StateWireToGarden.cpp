#include <EEPROMVariables.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/StateWireToGarden.h>
#include <config.h>



long int startTimeWireToGarden = 0;
long int currentTimeWireToGarden = 0;
long int lastTimeCheckedWireToGardenRotation = 0;

void read_wireToGarden_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMEvent = currentFSMEvent;
        TriggerFSM(STATE_WIRE_TO_GARDEN, STATE_PARKED, currentFSMSequence);
        return;
    }
}

void wireToGarden_on_enter() {
    clearLCD();
    lcd.setCursor(0, 0);
    lcd.print("WIRE TO GARDEN                 ");
    delay(500);
    clearLCD();
    startTimeWireToGarden = millis();
    currentTimeWireToGarden = startTimeWireToGarden;
    lastTimeCheckedWireToGardenRotation = startTimeWireToGarden;
    MotorAction_GoSlowSpeed();
}

void wireToGarden() {

    read_wireToGarden_keys();
    lcd.setCursor(0, 0);
    lcd.print("wireToGarden                     ");

    currentTimeWireToGarden = millis();

    if (currentTimeWireToGarden - startTimeWireToGarden < eeprom_rotate_from_wire_to_garden_time) {
        if (eeprom_perimeter_is_clockwise_from_garage == 1) {
            MotorAction_SetPinsToRotateLeft();
        } else {
            MotorAction_SetPinsToRotateRight();
        }
    } else {
        MotorAction_SetPinsToGoForward();
    }

    if ((currentTimeWireToGarden - startTimeWireToGarden) >= (eeprom_wire_into_garden_time + eeprom_rotate_from_wire_to_garden_time)) {
        TriggerFSM(STATE_WIRE_TO_GARDEN, STATE_MOWING, currentFSMSequence);
        return;
    }

    lastTimeCheckedWireToGardenRotation = currentTimeWireToGarden;
}

void wireToGarden_on_exit() {
    MotorAction_StopMotors();
    clearLCD();
    startTimeWireToGarden = 0;
    currentTimeWireToGarden = 0;
}

State state_wireToGarden(&wireToGarden_on_enter, &wireToGarden, &wireToGarden_on_exit);
