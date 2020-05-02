#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/StateMowing.h>


void read_mowing_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        // trigger_fsm(EVENT_DOCKED__EXIT_GARAGE);
    }
}

// DOCKED-STATE
void mowing_on_enter() {
}
void mowing() {
    read_mowing_keys();
    lcd.setCursor(0, 0);
    lcd.print("mowing                ");
}
void mowing_on_exit() {
}

State state_mowing(&mowing_on_enter, &mowing, &mowing_on_exit);
