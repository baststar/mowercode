#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/StateMowing.h>


void read_mowing_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(FSMEVENT_MOWING__TO__PARKED, currentFSMSequence);
    }
}

void mowing_on_enter() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MOWING                    ");
    delay(1000);
    lcd.clear();
}

void mowing() {
    read_mowing_keys();
    lcd.setCursor(0, 0);
    lcd.print("mowing                  ");
}

void mowing_on_exit() {
    lcd.clear();
}

State state_mowing(&mowing_on_enter, &mowing, &mowing_on_exit);
