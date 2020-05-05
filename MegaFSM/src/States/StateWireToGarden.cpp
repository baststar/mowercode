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
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(BuildStateTransitionId(STATE_WIRE_TO_GARDEN, STATE_PARKED), currentFSMSequence);
        return;
    }
}

void wireToGarden_on_enter() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WIRE TO GARDEN                 ");
    delay(500);
    lcd.clear();
    startTimeWireToGarden = millis();
    currentTimeWireToGarden = startTimeWireToGarden;
    lastTimeCheckedWireToGardenRotation = startTimeWireToGarden;
    MotorAction_GoSlowSpeed();
}

void wireToGarden() {

    read_wireToGarden_keys();
    lcd.setCursor(0, 0);
    lcd.print("wireToGarden...                     ");

    currentTimeWireToGarden = millis();

    if (currentTimeWireToGarden - startTimeWireToGarden < ROTATE_FROM_WIRE_TO_GARDEN_TIME) {
        if (PERIMETER_IS_CLOCKWISE_FROM_GARAGE == true) {
            MotorAction_SetPinsToRotateLeft();
        } else {
            MotorAction_SetPinsToRotateRight();
        }
    } else {
        MotorAction_SetPinsToGoForward();
    }

    if ((currentTimeWireToGarden - startTimeWireToGarden) >= (WIRE_INTO_TO_GARDEN_TIME + ROTATE_FROM_WIRE_TO_GARDEN_TIME)) {
        Trigger_FSM(BuildStateTransitionId(STATE_WIRE_TO_GARDEN, STATE_MOWING), currentFSMSequence);
        return;
    }

    lastTimeCheckedWireToGardenRotation = currentTimeWireToGarden;
}

void wireToGarden_on_exit() {
    MotorAction_StopMotors();
    lcd.clear();
    startTimeWireToGarden = 0;
    currentTimeWireToGarden = 0;
}

State state_wireToGarden(&wireToGarden_on_enter, &wireToGarden, &wireToGarden_on_exit);
