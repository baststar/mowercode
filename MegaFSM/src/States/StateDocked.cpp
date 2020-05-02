#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/StateDocked.h>


void read_docked_keys() {
    Read_Membrane_Keys();
    if (StartKey_pressed == 0) {
        trigger_fsm(FSMEVENT_DOCKED__TO__DOCKED_MENU);
    }
}

// DOCKED-STATE
void docked_on_enter() {
}
void docked() {
    read_docked_keys();
    lcd.setCursor(0, 0);
    lcd.print("docked                ");
}
void docked_on_exit() {
}

State state_docked(&docked_on_enter, &docked, &docked_on_exit);
