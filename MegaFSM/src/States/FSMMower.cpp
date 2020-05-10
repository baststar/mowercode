
#include <Fsm.h>
#include <LCD.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/Menus/StateDockedMenu.h>
#include <States/Menus/StateParkedMenu.h>
#include <States/Menus/StateSettingsAlarmsMenu.h>
#include <States/Menus/StateSettingsMenu.h>
#include <States/Menus/StateSettingsMotorspeedsMenu.h>
#include <States/Menus/StateSettingsMowtimesMenu.h>
#include <States/Menus/StateSettingsPerimeterMenu.h>
#include <States/Menus/StateSettingsTimesMenu.h>
#include <States/Menus/StateTestMenu.h>
#include <States/StateCompassRotate.h>
#include <States/StateDocked.h>
#include <States/StateError.h>
#include <States/StateExitGarage.h>
#include <States/StateFindWireBackwards.h>
#include <States/StateFindWireForwards.h>
#include <States/StateFollowWire.h>
#include <States/StateMowing.h>
#include <States/StateParked.h>
#include <States/StateRandomRotate.h>
#include <States/StateRotateToWire.h>
#include <States/StateWireToGarden.h>
#include <config.h>
#include <map>


int currentFSMEvent = -1;
int lastFSMEvent = -1;
int beforeMenuFSMEvent = -1;
int currentFSMSequence = -1;
Fsm fsm_mower(&state_docked);

std::map<uint16_t, bool> eventList;

#define STATES_COUNT 21

State *allStates[STATES_COUNT] = {
    &state_docked,                  // 1
    &state_dockedMenu,              // 2
    &state_exitGarage,              // 3
    &state_mowing,                  // 4
    &state_followWire,              // 5
    &state_parked,                  // 6
    &state_parkedMenu,              // 7
    &state_randomRotate,            // 8
    &state_wireToGarden,            // 9
    &state_testMenu,                // 10
    &state_error,                   // 11
    &state_compassRotate,           // 12
    &state_rotateToWire,            // 13
    &state_findWireForwards,        // 14
    &state_findWireBackwards,       // 15
    &state_settingsMenu,            // 16
    &state_settingsMotorspeedsMenu, // 17
    &state_settingsTimesMenu,       // 18
    &state_settingsPerimeterMenu,   // 19
    &state_settingsMowtimesMenu,    // 20
    &state_settingsAlarmsMenu,      // 21
};

int BuildStateTransitionId(int state1, int state2) {
    char buffer[5];
    snprintf(buffer, sizeof(buffer), "%d%d%d", state1, 9, state2);
    return String(buffer).toInt();
}

void Loop_FSM() {
    fsm_mower.run_machine();
}

void TriggerFSM(int fromState, int toState, int sequence) {
    int transitionId = BuildStateTransitionId(fromState, toState);
    lastFSMEvent = currentFSMEvent;
    currentFSMEvent = transitionId;
    currentFSMSequence = sequence;

    if (eventList.find(transitionId) == eventList.end()) {
        eventList.insert(std::pair<uint16_t, bool>(transitionId, true));
        fsm_mower.add_transition(&*allStates[fromState], &*allStates[toState], transitionId, NULL);
    }

    fsm_mower.trigger(transitionId);
}
