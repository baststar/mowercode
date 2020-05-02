
#include <Fsm.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/StateCompassRotate.h>
#include <States/StateDocked.h>
#include <States/StateDockedMenu.h>
#include <States/StateExitGarage.h>
#include <States/StateFindWire.h>
#include <States/StateFollowWire.h>
#include <States/StateMowing.h>
#include <States/StateParked.h>
#include <States/StateParkedMenu.h>
#include <States/StateRandomRotate.h>
#include <States/StateTestMenu.h>
#include <States/StateWireToGarden.h>

int currentFSMEvent = NULL;
int lastFSMEvent = NULL;
Fsm fsm_mower(&state_docked);

void setup_fsm() {

    // docked
    fsm_mower.add_transition(&state_docked, &state_dockedMenu, FSMEVENT_DOCKED__TO__DOCKED_MENU, NULL);

    // dockedMenu
    fsm_mower.add_transition(&state_dockedMenu, &state_docked, FSMEVENT_DOCKED_MENU__TO__DOCKED, NULL);
    fsm_mower.add_transition(&state_dockedMenu, &state_exitGarage, FSMEVENT_DOCKED_MENU__TO__EXIT_GARAGE, NULL);
    fsm_mower.add_transition(&state_dockedMenu, &state_testMenu, FSMEVENT_DOCKED_MENU__TO__TEST_MENU, NULL);

    // testMenu
    fsm_mower.add_transition(&state_testMenu, &state_dockedMenu, FSMEVENT_TEST_MENU__TO__DOCKED_MENU, NULL);

    // exitGarage
    fsm_mower.add_transition(&state_exitGarage, &state_mowing, FSMEVENT_EXIT_GARAGE__TO__MOWING, NULL);
    fsm_mower.add_transition(&state_exitGarage, &state_parked, FSMEVENT_EXIT_GARAGE__TO__PARKED, NULL);

    // mowing
    fsm_mower.add_transition(&state_mowing, &state_findWire, FSMEVENT_MOWING__TO__FIND_WIRE, NULL);
    fsm_mower.add_transition(&state_mowing, &state_parked, FSMEVENT_MOWING__TO__PARKED, NULL);
    fsm_mower.add_transition(&state_mowing, &state_randomRotate, FSMEVENT_MOWING__TO__RANDOM_ROTATE, NULL);

    // parked
    fsm_mower.add_transition(&state_parked, &state_parkedMenu, FSMEVENT_PARKED__TO__PARKED_MENU, NULL);

    // parkedMenu
    fsm_mower.add_transition(&state_parkedMenu, &state_parked, FSMEVENT_PARKED_MENU__TO__PARKED, NULL);
    fsm_mower.add_transition(&state_parkedMenu, &state_followWire, FSMEVENT_PARKED_MENU__TO__FOLLOW_WIRE, NULL);
    fsm_mower.add_transition(&state_parkedMenu, &state_findWire, FSMEVENT_PARKED_MENU__TO__FIND_WIRE, NULL);
    fsm_mower.add_transition(&state_parkedMenu, &state_randomRotate, FSMEVENT_PARKED_MENU__TO__RANDOM_ROTATE, NULL);

    // findWire
    fsm_mower.add_transition(&state_findWire, &state_followWire, FSMEVENT_FIND_WIRE__TO__FOLLOW_WIRE, NULL);
    fsm_mower.add_transition(&state_findWire, &state_parked, FSMEVENT_FIND_WIRE__TO__PARKED, NULL);

    // followWire
    fsm_mower.add_transition(&state_followWire, &state_docked, FSMEVENT_FOLLOW_WIRE__TO__DOCKED, NULL);
    fsm_mower.add_transition(&state_followWire, &state_parked, FSMEVENT_FOLLOW_WIRE__TO__PARKED, NULL);

    // randomRotate
    fsm_mower.add_transition(&state_randomRotate, &state_mowing, FSMEVENT_RANDOM_ROTATE__TO__MOWING, NULL);
    fsm_mower.add_transition(&state_randomRotate, &state_parked, FSMEVENT_RANDOM_ROTATE__TO__MOWING, NULL);

    // wireToGarden
    fsm_mower.add_transition(&state_wireToGarden, &state_parked, FSMEVENT_WIRE_TO_GARDEN__TO__PARKED, NULL);
    fsm_mower.add_transition(&state_wireToGarden, &state_mowing, FSMEVENT_WIRE_TO_GARDEN__TO__MOWING, NULL);
}

void loop_fsm() {
    fsm_mower.run_machine();
    delay(100);
}

void trigger_fsm(int event) {
    lastFSMEvent = currentFSMEvent;
    currentFSMEvent = event;
    fsm_mower.trigger(event);
}
