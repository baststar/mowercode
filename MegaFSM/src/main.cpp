#include <Arduino.h>
#include <LCD.h>
#include <SetupPins.h>
#include <States/FSMMower.h>

void setup() {
    Setup_All_Pins();
    Setup_LCD();
    setup_fsm();
}

void loop() {
    loop_fsm();
}
