#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/StateParked.h>

void read_parked_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        // trigger_fsm(EVENT_DOCKED__EXIT_GARAGE);
    }
}

// DOCKED-STATE
void parked_on_enter() {
}
void parked() {
    read_parked_keys();
    lcd.setCursor(0, 0);
    lcd.print("parked                ");
}
void parked_on_exit() {
}

State state_parked(&parked_on_enter, &parked, &parked_on_exit);