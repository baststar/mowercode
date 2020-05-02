#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/StateFindWire.h>


void read_findWire_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        // trigger_fsm(EVENT_DOCKED__EXIT_GARAGE);
    }
}

// DOCKED-STATE
void findWire_on_enter() {
}
void findWire() {
    read_findWire_keys();
    lcd.setCursor(0, 0);
    lcd.print("findWire                ");
}
void findWire_on_exit() {
}

State state_findWire(&findWire_on_enter, &findWire, &findWire_on_exit);
