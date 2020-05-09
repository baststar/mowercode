#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/StateCompassRotate.h>

void read_compassRotate_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMEvent = currentFSMEvent;
        TriggerFSM(STATE_COMPASS_ROTATE, STATE_PARKED, currentFSMSequence);
        return;
    }
}

void compassRotate_on_enter() {
    lcd.setCursor(0, 0);
    lcd.print("COMPASS ROTATE                 ");
    delay(500);
    clearLCD();
}

void compassRotate() {
    read_compassRotate_keys();
    lcd.setCursor(0, 0);
    lcd.print("compassRotate                   ");
}

void compassRotate_on_exit() {
    clearLCD();
}
State state_compassRotate(&compassRotate_on_enter, &compassRotate, &compassRotate_on_exit);
