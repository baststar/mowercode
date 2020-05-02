#include <Arduino.h>
#include <Keyboard.h>
#include <config.h>
#include <declarations.h>

// KEYBOARD-EVENTS
bool StartKey_pressed = 0;
bool StartKey_lastState = 1;
bool PlusKey_pressed = 0;
bool PlusKey_lastState = 1;
bool MinusKey_pressed = 0;
bool MinusKey_lastState = 1;
bool StopKey_pressed = 0;
bool StopKey_lastState = 1;

void Read_Membrane_Keys() {
    StartKey_pressed = digitalRead(PIN_START_KEY);
    PlusKey_pressed = digitalRead(PIN_PLUS_KEY);
    MinusKey_pressed = digitalRead(PIN_MINUS_KEY);
    StopKey_pressed = digitalRead(PIN_STOP_KEY);
}
