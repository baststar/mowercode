#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/StateDocked.h>



void read_docked_keys() {
    Read_Membrane_Keys();
    if (StartKey_pressed == 0) {
        int stateId = (String(STATE_DOCKED) + String(9999) + String(STATE_DOCKED_MENU)).toInt();
        Trigger_FSM(stateId, -1);
        return;
    }
}

void docked_on_enter() {
    MotorAction_StopBlades();
    MotorAction_StopMotors();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DOCKED                    ");
    delay(500);
    lcd.clear();
}
void docked() {
    read_docked_keys();
    lcd.setCursor(0, 0);
    lcd.print("docked...                        ");
}
void docked_on_exit() {
    lcd.clear();
}

State state_docked(&docked_on_enter, &docked, &docked_on_exit);
