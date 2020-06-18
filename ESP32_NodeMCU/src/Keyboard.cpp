#include <Arduino.h>
#include <Keyboard.h>
#include <config.h>

// KEYBOARD-EVENTS
bool StartKey_pressed = 0;
bool PlusKey_pressed = 0;
bool MinusKey_pressed = 0;
bool StopKey_pressed = 0;

void Read_Membrane_Keys() {
    StartKey_pressed = digitalRead(PIN_START_KEY);
    PlusKey_pressed = digitalRead(PIN_PLUS_KEY);
    MinusKey_pressed = digitalRead(PIN_MINUS_KEY);
    StopKey_pressed = digitalRead(PIN_STOP_KEY);

    // Serial.println(String(StartKey_pressed) + ' ' + String(PlusKey_pressed) + ' ' + String(MinusKey_pressed) + ' ' + String(StopKey_pressed));
}
