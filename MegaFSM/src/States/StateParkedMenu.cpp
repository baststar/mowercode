#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/StateParkedMenu.h>
#include <config.h>
#include <vector>

using namespace std;

int parkedMenu_currentMenu = 0;

vector<String> parkedMenuNames = {"Continue", "To garage"};

void read_parkedMenu_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(BuildStateTransitionId(STATE_PARKED_MENU, STATE_PARKED), currentFSMSequence);
        return;
    } else if (PlusKey_pressed == 0) {
        delay(250);
        parkedMenu_currentMenu++;
        if (parkedMenu_currentMenu >= parkedMenuNames.size()) {
            parkedMenu_currentMenu = 0;
        }
    } else if (MinusKey_pressed == 0) {
        delay(250);
        parkedMenu_currentMenu--;
        if (parkedMenu_currentMenu < 0) {
            parkedMenu_currentMenu = parkedMenuNames.size() - 1;
        }
    } else if (StartKey_pressed == 0) {
        delay(250);
        if (parkedMenu_currentMenu == 0) {
            clearLCD();
            lcd.setCursor(0, 0);
            lcd.print("continue " + String(beforeMenuFSMEvent));
            delay(500);
            clearLCD();
            Trigger_FSM(beforeMenuFSMEvent, currentFSMSequence);
            return;
        } else if (parkedMenu_currentMenu == 1) {
            Trigger_FSM(BuildStateTransitionId(STATE_PARKED_MENU, STATE_FIND_WIRE_FORWARDS), FSMSEQUENCE_FOLLOW_WIRE);
            return;
        }
    }
}

void parkedMenu_on_enter() {
    clearLCD();
    lcd.setCursor(0, 0);
    lcd.print("PARKED-MENU                 ");
    delay(500);
    clearLCD();
    parkedMenu_currentMenu = 0;
}
void parkedMenu() {
    String menuname = GetMenuName(parkedMenuNames, parkedMenu_currentMenu);
    lcd.setCursor(0, 0);
    lcd.print(menuname + "           ");
    read_parkedMenu_keys();
}
void parkedMenu_on_exit() {
    clearLCD();
}
State state_parkedMenu(&parkedMenu_on_enter, &parkedMenu, &parkedMenu_on_exit);
