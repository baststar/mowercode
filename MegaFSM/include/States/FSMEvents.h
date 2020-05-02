#pragma once

// EVENTS

// DOCKED 10
#define FSMEVENT_DOCKED__TO__DOCKED_MENU 10

// DOCKED MENU 20
#define FSMEVENT_DOCKED_MENU__TO__DOCKED 20
#define FSMEVENT_DOCKED_MENU__TO__EXIT_GARAGE 21
#define FSMEVENT_DOCKED_MENU__TO__TEST_MENU 22

// EXIT GARAGE 30
#define FSMEVENT_EXIT_GARAGE__TO__MOWING 30
#define FSMEVENT_EXIT_GARAGE__TO__PARKED 31

// MOWING 40
#define FSMEVENT_MOWING__TO__FIND_WIRE 40
#define FSMEVENT_MOWING__TO__PARKED 41
#define FSMEVENT_MOWING__TO__RANDOM_ROTATE 42

// FIND WIRE 50
#define FSMEVENT_FIND_WIRE__TO__FOLLOW_WIRE 50
#define FSMEVENT_FIND_WIRE__TO__PARKED 51

// FOLLOW WIRE 60
#define FSMEVENT_FOLLOW_WIRE__TO__DOCKED 60
#define FSMEVENT_FOLLOW_WIRE__TO__PARKED 61

// PARKED 70
#define FSMEVENT_PARKED__TO__PARKED_MENU 70

// PARKED MENU 80
#define FSMEVENT_PARKED_MENU__TO__PARKED 80
#define FSMEVENT_PARKED_MENU__TO__FOLLOW_WIRE 71
#define FSMEVENT_PARKED_MENU__TO__FIND_WIRE 73
#define FSMEVENT_PARKED_MENU__TO__RANDOM_ROTATE 74

// RANDOM ROTATE 90
#define FSMEVENT_RANDOM_ROTATE__TO__MOWING 90
#define FSMEVENT_RANDOM_ROTATE__TO__PARKED 91

// WIRE TO GARDEN 100
#define FSMEVENT_WIRE_TO_GARDEN__TO__PARKED 100
#define FSMEVENT_WIRE_TO_GARDEN__TO__MOWING 101

// TEST MENU 110
#define FSMEVENT_TEST_MENU__TO__DOCKED_MENU 110