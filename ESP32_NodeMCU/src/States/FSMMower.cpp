#include <Fsm.h>
#include <LCD.h>
#include <NodeMCU.h>
#include <Perimeter.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/FSMStates.h>
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
#include <VoltAmpRain.h>
#include <WLan.h>
#include <config.h>
#include <map>




unsigned long currentMillisGlobal = 0;

int currentFSMTransition = -1;
int lastFSMTransition = -1;
int beforeMenuFSMTransition = -1;
int currentFSMSequence = -1;
int currentState = 0;
int lastState = -1;
unsigned long sequenceStartTime = 0;
Fsm fsm_mower(&state_docked);

std::map<int, bool> eventList;

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

char *mowerStateNames[] = {"STATE_DOCKED",
                           "STATE_DOCKED_MENU",
                           "STATE_EXIT_GARAGE",
                           "STATE_MOWING",
                           "STATE_FOLLOW_WIRE",
                           "STATE_PARKED",
                           "STATE_PARKED_MENU",
                           "STATE_RANDOM_ROTATE",
                           "STATE_WIRE_TO_GARDEN",
                           "STATE_TEST_MENU",
                           "STATE_ERROR",
                           "STATE_COMPASS_ROTATE",
                           "STATE_ROTATE_TO_WIRE",
                           "STATE_FIND_WIRE_FORWARDS",
                           "STATE_FIND_WIRE_BACKWARDS",
                           "STATE_SETTINGS_MENU",
                           "STATE_SETTINGS_MOTORSPEED_MENU",
                           "STATE_SETTINGS_TIMES_MENU",
                           "STATE_SETTINGS_PERIMETER_MENU",
                           "STATE_SETTINGS_MOWTIMES_MENU",
                           "STATE_SETTINGS_ALARMS_MENU"};

int BuildStateTransitionId(int state1, int state2) {
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%d9%d", state1, state2);
    int number = String(buffer).toInt();
    return number;
}

void Loop_FSM() {
    currentMillisGlobal = millis();
    // UpdateVoltAmpRain();
    // UpdatePerimeterStatus();
    fsm_mower.run_machine();
}

void TriggerFSM(int fromState, int toState, int sequence) {
    int transitionId = BuildStateTransitionId(fromState, toState);
    lastFSMTransition = currentFSMTransition;
    currentFSMTransition = transitionId;
    currentFSMSequence = sequence;
    lastState = fromState;
    currentState = toState;

    // SendData("mower", );
    SendMQTTMessage("mower", mowerStateNames[toState - 1]);

    if (eventList.find(transitionId) == eventList.end()) {
        eventList.insert(std::pair<int, bool>(transitionId, true));
        fsm_mower.add_transition(&*allStates[fromState - 1], &*allStates[toState - 1], transitionId, NULL);
    }
    fsm_mower.trigger(transitionId);
}

void StartSequence(int sequence) {
    sequenceStartTime = currentMillisGlobal;
    if (sequence == FSMSEQUENCE_ZONE_1 || sequence == FSMSEQUENCE_ZONE_2 || sequence == FSMSEQUENCE_QUICK_MOW) {
        if (currentState == STATE_DOCKED || currentState == STATE_DOCKED_MENU) {
            TriggerFSM(currentState, STATE_EXIT_GARAGE, sequence);
        }
    } else if (sequence == FSMSEQUENCE_GO_TO_DOCK) {
        if (currentState == STATE_MOWING || currentState == STATE_DOCKED_MENU) {
            TriggerFSM(currentState, STATE_FIND_WIRE_FORWARDS, sequence);
        }
    }
}
