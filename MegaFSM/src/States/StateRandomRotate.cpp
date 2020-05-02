#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/StateRandomRotate.h>


void read_randomRotate_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        // trigger_fsm(EVENT_DOCKED__EXIT_GARAGE);
    }
}

// DOCKED-STATE
void randomRotate_on_enter() {
}
void randomRotate() {
    read_randomRotate_keys();
    lcd.setCursor(0, 0);
    lcd.print("randomRotate                ");
}
void randomRotate_on_exit() {
}

State state_randomRotate(&randomRotate_on_enter, &randomRotate, &randomRotate_on_exit);
