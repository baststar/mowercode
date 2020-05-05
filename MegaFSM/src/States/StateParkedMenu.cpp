#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/StateParkedMenu.h>
#include <config.h>


int parkedMenu_currentMenu = 0;
const int parkedMenuArraySize = 2;
String parkedMenuNames[parkedMenuArraySize] = {"Continue", "To garage"};

void read_parkedMenu_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        beforeMenuFSMEvent = currentFSMEvent;
        int stateId = (String(STATE_PARKED_MENU) + String(9999) + String(STATE_PARKED)).toInt();
        Trigger_FSM(stateId, currentFSMSequence);
        return;
    } else if (PlusKey_pressed == 0) {
        delay(100);
        parkedMenu_currentMenu++;
        if (parkedMenu_currentMenu >= parkedMenuArraySize) {
            parkedMenu_currentMenu = 0;
        }
    } else if (MinusKey_pressed == 0) {
        delay(100);
        parkedMenu_currentMenu--;
        if (parkedMenu_currentMenu < 0) {
            parkedMenu_currentMenu = parkedMenuArraySize - 1;
        }
    } else if (StartKey_pressed == 0) {
        delay(100);
        if (parkedMenu_currentMenu == 0) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("continue " + String(beforeMenuFSMEvent));
            delay(500);
            lcd.clear();
            Trigger_FSM(beforeMenuFSMEvent, currentFSMSequence);
            return;
        } else if (parkedMenu_currentMenu == 1) {
            int stateId = (String(STATE_PARKED_MENU) + String(9999) + String(STATE_FIND_WIRE_FORWARDS)).toInt();
            Trigger_FSM(stateId, FSMSEQUENCE_FOLLOW_WIRE);
            return;
        }
    }
}

void parkedMenu_on_enter() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PARKED-MENU                 ");
    delay(500);
    lcd.clear();
    parkedMenu_currentMenu = 0;
}
void parkedMenu() {
    read_parkedMenu_keys();
    Show_Parked_Menu();
}
void parkedMenu_on_exit() {
    lcd.clear();
}
State state_parkedMenu(&parkedMenu_on_enter, &parkedMenu, &parkedMenu_on_exit);
