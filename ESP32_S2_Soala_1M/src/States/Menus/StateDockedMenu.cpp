#include <CustomFunctions.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/FSMStates.h>
#include <States/Menus/StateDockedMenu.h>



int dockedMenu_currentMenu = 0;
String dockedMenuNames[] = {"Mow from Zone 1", "Mow from Zone 2", "Quick mowing", "Tests...", "Settings...", "To garage"};

void read_dockedMenu_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMTransition = currentFSMTransition;
        TriggerFSM(STATE_DOCKED_MENU, STATE_DOCKED, -1);
        return;
    } else if (PlusKey_pressed == 0) {
        delay(250);
        dockedMenu_currentMenu++;
        if (dockedMenu_currentMenu >= ARRAY_SIZE(dockedMenuNames)) {
            dockedMenu_currentMenu = 0;
        }
    } else if (MinusKey_pressed == 0) {
        delay(250);
        dockedMenu_currentMenu--;
        if (dockedMenu_currentMenu < 0) {
            dockedMenu_currentMenu = ARRAY_SIZE(dockedMenuNames) - 1;
        }
    } else if (StartKey_pressed == 0) {
        delay(250);
        if (dockedMenu_currentMenu == 0) {
            StartSequence(FSMSEQUENCE_ZONE_1);
            return;
        } else if (dockedMenu_currentMenu == 1) {
            StartSequence(FSMSEQUENCE_ZONE_2);
            return;
        } else if (dockedMenu_currentMenu == 2) {
            StartSequence(FSMSEQUENCE_QUICK_MOW);
            return;
        } else if (dockedMenu_currentMenu == 3) {
            TriggerFSM(STATE_DOCKED_MENU, STATE_TEST_MENU, -1);
            return;
        } else if (dockedMenu_currentMenu == 4) {
            TriggerFSM(STATE_DOCKED_MENU, STATE_SETTINGS_MENU, -1);
            return;
        } else if (dockedMenu_currentMenu == 5) {
            StartSequence(FSMSEQUENCE_GO_TO_DOCK);
            return;
        }
    }
}

void dockedMenu_on_enter() {
    dockedMenu_currentMenu = 0;
}
void dockedMenu() {
    String menuname = dockedMenuNames[dockedMenu_currentMenu];
    lcd.setCursor(0, 0);
    lcd.print(menuname + "           ");
    read_dockedMenu_keys();
}
void dockedMenu_on_exit() {
    clearLCD();
}
State state_dockedMenu(&dockedMenu_on_enter, &dockedMenu, &dockedMenu_on_exit);
