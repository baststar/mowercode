#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/StateWireToGarden.h>


void read_wireToGarden_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        // trigger_fsm(EVENT_DOCKED__EXIT_GARAGE);
    }
}

// DOCKED-STATE
void wireToGarden_on_enter() {
}
void wireToGarden() {
    read_wireToGarden_keys();
    lcd.setCursor(0, 0);
    lcd.print("wireToGarden              ");
}
void wireToGarden_on_exit() {
}

State state_wireToGarden(&wireToGarden_on_enter, &wireToGarden, &wireToGarden_on_exit);
