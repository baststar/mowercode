
#include <Fsm.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/StateCompassRotate.h>
#include <States/StateDocked.h>
#include <States/StateDockedMenu.h>
#include <States/StateError.h>
#include <States/StateExitGarage.h>
#include <States/StateFindWireBackwards.h>
#include <States/StateFindWireForwards.h>
#include <States/StateFollowWire.h>
#include <States/StateMowing.h>
#include <States/StateParked.h>
#include <States/StateParkedMenu.h>
#include <States/StateRandomRotate.h>
#include <States/StateRotateToWire.h>
#include <States/StateTestMenu.h>
#include <States/StateWireToGarden.h>
#include <config.h>


int currentFSMEvent = NULL;
int lastFSMEvent = NULL;
int beforeMenuFSMEvent = NULL;
int currentFSMSequence = NULL;
Fsm fsm_mower(&state_docked);

#define STATES_COUNT 15

State *allStates[STATES_COUNT] = {
    &state_docked,           // 1
    &state_dockedMenu,       // 2
    &state_exitGarage,       // 3
    &state_mowing,           // 4
    &state_followWire,       // 5
    &state_parked,           // 6
    &state_parkedMenu,       // 7
    &state_randomRotate,     // 8
    &state_wireToGarden,     // 9
    &state_testMenu,         // 10
    &state_error,            // 11
    &state_compassRotate,    // 12
    &state_rotateToWire,     // 13
    &state_findWireForwards, // 14
    &state_findWireBackwards // 15
};

int BuildStateTransitionId(int state1, int state2) {
    char transitionId[5];
    sprintf(transitionId, "%d%d%d", state1, 9, state2);
    return String(transitionId).toInt();
}

void Setup_FSM() {
    // create any to any transitions
    for (unsigned int i = 1; i <= STATES_COUNT; i++) {
        State currentState = State(*allStates[i - 1]);
        for (unsigned int j = 1; j <= STATES_COUNT; j++) {
            State tmpState = State(*allStates[j - 1]);
            fsm_mower.add_transition(&*allStates[i - 1], &*allStates[j - 1], BuildStateTransitionId(i, j), NULL);
        }
    }
}

void Loop_FSM() {
    fsm_mower.run_machine();
}

void Trigger_FSM(int event, int sequence) {
    Serial.println("change state to event: " + String(event) + " Sequence: " + String(sequence));
    lastFSMEvent = currentFSMEvent;
    currentFSMEvent = event;
    currentFSMSequence = sequence;
    fsm_mower.trigger(event);
}
