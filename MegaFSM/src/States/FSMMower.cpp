
#include <Fsm.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/StateCompassRotate.h>
#include <States/StateDocked.h>
#include <States/StateDockedMenu.h>
#include <States/StateError.h>
#include <States/StateExitGarage.h>
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

void Setup_FSM() {

    // docked
    fsm_mower.add_transition(&state_docked, &state_dockedMenu, FSMEVENT_DOCKED__TO__DOCKED_MENU, NULL);
    fsm_mower.add_transition(&state_docked, &state_error, FSMEVENT_DOCKED__TO__ERROR, NULL);

    // dockedMenu
    fsm_mower.add_transition(&state_dockedMenu, &state_docked, FSMEVENT_DOCKED_MENU__TO__DOCKED, NULL);
    fsm_mower.add_transition(&state_dockedMenu, &state_exitGarage, FSMEVENT_DOCKED_MENU__TO__EXIT_GARAGE, NULL);
    fsm_mower.add_transition(&state_dockedMenu, &state_testMenu, FSMEVENT_DOCKED_MENU__TO__TEST_MENU, NULL);
    fsm_mower.add_transition(&state_dockedMenu, &state_error, FSMEVENT_DOCKED_MENU__TO__ERROR, NULL);

    // testMenu
    fsm_mower.add_transition(&state_testMenu, &state_dockedMenu, FSMEVENT_TEST_MENU__TO__DOCKED_MENU, NULL);
    fsm_mower.add_transition(&state_testMenu, &state_error, FSMEVENT_TEST_MENU__TO__ERROR, NULL);

    // exitGarage
    fsm_mower.add_transition(&state_exitGarage, &state_mowing, FSMEVENT_EXIT_GARAGE__TO__MOWING, NULL);
    fsm_mower.add_transition(&state_exitGarage, &state_parked, FSMEVENT_EXIT_GARAGE__TO__PARKED, NULL);
    fsm_mower.add_transition(&state_exitGarage, &state_randomRotate, FSMEVENT_EXIT_GARAGE__TO__RANDOM_ROTATE, NULL);
    fsm_mower.add_transition(&state_exitGarage, &state_findWire, FSMEVENT_EXIT_GARAGE__TO__FIND_WIRE, NULL);
    fsm_mower.add_transition(&state_exitGarage, &state_rotateToWire, FSMEVENT_EXIT_GARAGE__TO__ROTATE_TO_WIRE, NULL);
    fsm_mower.add_transition(&state_exitGarage, &state_error, FSMEVENT_EXIT_GARAGE__TO__ERROR, NULL);

    // mowing
    fsm_mower.add_transition(&state_mowing, &state_findWire, FSMEVENT_MOWING__TO__FIND_WIRE, NULL);
    fsm_mower.add_transition(&state_mowing, &state_parked, FSMEVENT_MOWING__TO__PARKED, NULL);
    fsm_mower.add_transition(&state_mowing, &state_randomRotate, FSMEVENT_MOWING__TO__RANDOM_ROTATE, NULL);
    fsm_mower.add_transition(&state_mowing, &state_error, FSMEVENT_MOWING__TO__ERROR, NULL);

    // parked
    fsm_mower.add_transition(&state_parked, &state_parkedMenu, FSMEVENT_PARKED__TO__PARKED_MENU, NULL);
    fsm_mower.add_transition(&state_parked, &state_error, FSMEVENT_PARKED__TO__ERROR, NULL);

    // parkedMenu
    fsm_mower.add_transition(&state_parkedMenu, &state_parked, FSMEVENT_PARKED_MENU__TO__PARKED, NULL);
    fsm_mower.add_transition(&state_parkedMenu, &state_followWire, FSMEVENT_PARKED_MENU__TO__FOLLOW_WIRE, NULL);
    fsm_mower.add_transition(&state_parkedMenu, &state_findWire, FSMEVENT_PARKED_MENU__TO__FIND_WIRE, NULL);
    fsm_mower.add_transition(&state_parkedMenu, &state_randomRotate, FSMEVENT_PARKED_MENU__TO__RANDOM_ROTATE, NULL);
    fsm_mower.add_transition(&state_parkedMenu, &state_error, FSMEVENT_PARKED_MENU__TO__ERROR, NULL);
    // continue from parking
    fsm_mower.add_transition(&state_parkedMenu, &state_exitGarage, FSMEVENT_DOCKED_MENU__TO__EXIT_GARAGE, NULL);
    fsm_mower.add_transition(&state_parkedMenu, &state_mowing, FSMEVENT_EXIT_GARAGE__TO__MOWING, NULL);
    fsm_mower.add_transition(&state_parkedMenu, &state_mowing, FSMEVENT_WIRE_TO_GARDEN__TO__MOWING, NULL);
    fsm_mower.add_transition(&state_parkedMenu, &state_randomRotate, FSMEVENT_EXIT_GARAGE__TO__RANDOM_ROTATE, NULL);
    fsm_mower.add_transition(&state_parkedMenu, &state_randomRotate, FSMEVENT_MOWING__TO__RANDOM_ROTATE, NULL);
    fsm_mower.add_transition(&state_parkedMenu, &state_findWire, FSMEVENT_EXIT_GARAGE__TO__FIND_WIRE, NULL);
    fsm_mower.add_transition(&state_parkedMenu, &state_findWire, FSMEVENT_MOWING__TO__FIND_WIRE, NULL);
    fsm_mower.add_transition(&state_parkedMenu, &state_findWire, FSMEVENT_ROTATE_TO_WIRE__TO__FIND_WIRE, NULL);
    fsm_mower.add_transition(&state_parkedMenu, &state_followWire, FSMEVENT_FIND_WIRE__TO__FOLLOW_WIRE, NULL);
    fsm_mower.add_transition(&state_parkedMenu, &state_wireToGarden, FSMEVENT_FOLLOW_WIRE__TO__WIRE_TO_GARDEN, NULL);
    fsm_mower.add_transition(&state_parkedMenu, &state_rotateToWire, FSMEVENT_EXIT_GARAGE__TO__ROTATE_TO_WIRE, NULL);

    // findWire
    fsm_mower.add_transition(&state_findWire, &state_followWire, FSMEVENT_FIND_WIRE__TO__FOLLOW_WIRE, NULL);
    fsm_mower.add_transition(&state_findWire, &state_parked, FSMEVENT_FIND_WIRE__TO__PARKED, NULL);
    fsm_mower.add_transition(&state_findWire, &state_error, FSMEVENT_FIND_WIRE__TO__ERROR, NULL);

    // followWire
    fsm_mower.add_transition(&state_followWire, &state_docked, FSMEVENT_FOLLOW_WIRE__TO__DOCKED, NULL);
    fsm_mower.add_transition(&state_followWire, &state_parked, FSMEVENT_FOLLOW_WIRE__TO__PARKED, NULL);
    fsm_mower.add_transition(&state_followWire, &state_error, FSMEVENT_FOLLOW_WIRE__TO__ERROR, NULL);
    fsm_mower.add_transition(&state_followWire, &state_wireToGarden, FSMEVENT_FOLLOW_WIRE__TO__WIRE_TO_GARDEN, NULL);

    // wireToGarden
    fsm_mower.add_transition(&state_wireToGarden, &state_mowing, FSMEVENT_WIRE_TO_GARDEN__TO__MOWING, NULL);
    fsm_mower.add_transition(&state_wireToGarden, &state_parked, FSMEVENT_WIRE_TO_GARDEN__TO__PARKED, NULL);
    fsm_mower.add_transition(&state_wireToGarden, &state_error, FSMEVENT_WIRE_TO_GARDEN__TO__ERROR, NULL);

    // randomRotate
    fsm_mower.add_transition(&state_randomRotate, &state_mowing, FSMEVENT_RANDOM_ROTATE__TO__MOWING, NULL);
    fsm_mower.add_transition(&state_randomRotate, &state_parked, FSMEVENT_RANDOM_ROTATE__TO__PARKED, NULL);
    fsm_mower.add_transition(&state_randomRotate, &state_error, FSMEVENT_RANDOM_ROTATE__TO__ERROR, NULL);

    // wireToGarden
    fsm_mower.add_transition(&state_wireToGarden, &state_mowing, FSMEVENT_WIRE_TO_GARDEN__TO__MOWING, NULL);
    fsm_mower.add_transition(&state_wireToGarden, &state_parked, FSMEVENT_WIRE_TO_GARDEN__TO__PARKED, NULL);
    fsm_mower.add_transition(&state_wireToGarden, &state_error, FSMEVENT_WIRE_TO_GARDEN__TO__ERROR, NULL);

    // rotateToWire
    fsm_mower.add_transition(&state_rotateToWire, &state_findWire, FSMEVENT_ROTATE_TO_WIRE__TO__FIND_WIRE, NULL);
    fsm_mower.add_transition(&state_rotateToWire, &state_parked, FSMEVENT_ROTATE_TO_WIRE__TO__PARKED, NULL);
    fsm_mower.add_transition(&state_rotateToWire, &state_error, FSMEVENT_ROTATE_TO_WIRE__TO__ERROR, NULL);

    // error
    fsm_mower.add_transition(&state_error, &state_parked, FSMEVENT_ERROR__TO__PARKED, NULL);
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
