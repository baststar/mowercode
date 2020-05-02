#pragma once

void setup_fsm();
void loop_fsm();
void trigger_fsm(int event);
void check_key_pressed();
void getChanges();


// EVENTS
#define EVENT_DOCKED__EXIT_GARAGE 1
#define EVENT_DOCKED__DOCKED_MENU 2

#define EVENT_DOCKED_MENU__EXIT_GARAGE 3
#define EVENT_DOCKED_MENU__TEST_MENU 4

#define EVENT_EXIT_GARAGE__DOCKED 10

// #define EXIT_GARAGE__FIND_WIRE 2