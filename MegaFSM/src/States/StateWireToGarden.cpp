#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/StateWireToGarden.h>
#include <config.h>


long int startTimeWireToGarden = 0;
long int currentTimeWireToGarden = 0;

void read_wireToGarden_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(FSMEVENT_WIRE_TO_GARDEN__TO__PARKED, currentFSMSequence);
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
}

void wireToGarden() {
    read_wireToGarden_keys();
    lcd.setCursor(0, 0);
    lcd.print("wireToGarden...                     ");

    currentTimeWireToGarden = millis();

    if ((currentTimeWireToGarden - startTimeWireToGarden) >= WIRE_TO_GARDEN_TIME) {
        Trigger_FSM(FSMEVENT_WIRE_TO_GARDEN__TO__MOWING, currentFSMSequence);
    }
}

void wireToGarden_on_exit() {
    lcd.clear();
    startTimeWireToGarden = 0;
    currentTimeWireToGarden = 0;
}

State state_wireToGarden(&wireToGarden_on_enter, &wireToGarden, &wireToGarden_on_exit);
