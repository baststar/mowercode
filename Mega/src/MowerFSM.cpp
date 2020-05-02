
#include <Fsm.h>
#include <LCD_Display.h>
#include <Menu_Docked.h>
#include <MowerFSM.h>
#include <declarations.h>

void on_dockedmenu_on_enter() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("dockedmenu enter");
    delay(1000);
}

void on_dockedmenu() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("dockedmenu");
}

void on_dockedmenu_on_exit() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("dockedmenu exit");
    delay(1000);
}
State state_dockedmenu_on(&on_dockedmenu_on_enter, &on_dockedmenu, &on_dockedmenu_on_exit);

void on_docked_on_enter() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("docked_on_enter");
    delay(1000);
}
void on_docked() {
    getChanges();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("docked");
}
void on_docked_on_exit() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("docked_on_exit");
    delay(1000);
}
void on_trans_docked_enter_docked_exit() {
}
State state_docked_on(&on_docked_on_enter, &on_docked, &on_docked_on_exit);


void on_exitgarage_on_enter() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("exitgarage_on_enter");
    delay(1000);
}
void on_exitgarage() {
    getChanges();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("exitgarage");
}
void on_exitgarage_on_exit() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("exitgarage_on_exit");
    delay(1000);
}
void on_trans_exitgarage_enter_exitgarage_exit() {
}
State state_exitdock_on(&on_exitgarage_on_enter, &on_exitgarage, &on_exitgarage_on_exit);

void check_key_pressed() {
    Read_Membrane_Keys();
    if (Start_Key_X == 0) {
        trigger_fsm(EVENT_DOCKED__EXIT_GARAGE);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Start_Key_X " + String(Start_Key_X));
    } else if (Stop_Key_X == 0) {
        trigger_fsm(EVENT_EXIT_GARAGE__DOCKED);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Stop_Key_X " + String(Stop_Key_X));
    }
}

void getChanges() {
    check_key_pressed();
}

Fsm fsm_mower(&state_docked_on);

void setup_fsm() {

    fsm_mower.add_transition(&state_docked_on, &state_exitdock_on, EVENT_DOCKED__EXIT_GARAGE, NULL);
    fsm_mower.add_transition(&state_exitdock_on, &state_dockedmenu_on, EVENT_DOCKED__DOCKED_MENU, NULL);

    fsm_mower.add_transition(&state_exitdock_on, &state_docked_on, EVENT_EXIT_GARAGE__DOCKED, NULL);
}

void loop_fsm() {
    fsm_mower.run_machine();
    delay(100);
}

void trigger_fsm(int event) {
    fsm_mower.trigger(event);
}
