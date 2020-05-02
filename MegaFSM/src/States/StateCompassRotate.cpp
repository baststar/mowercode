#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/StateCompassRotate.h>

void read_compassRotate_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        // trigger_fsm(EVENT_DOCKED__EXIT_GARAGE);
    }
}

// EXITGARAGE-STATE
void compassRotate_on_enter() {
}
void compassRotate() {
    read_compassRotate_keys();
    lcd.setCursor(0, 0);
    lcd.print("compassRotate               ");
}
void compassRotate_on_exit() {
}
State state_compassRotate(&compassRotate_on_enter, &compassRotate, &compassRotate_on_exit);
