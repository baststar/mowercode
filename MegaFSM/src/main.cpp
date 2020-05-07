#include <Arduino.h>
#include <Clock.h>
#include <LCD.h>
#include <Perimeter.h>
#include <SetupPins.h>
#include <States/FSMMower.h>

void setup() {
    Serial.begin(115200);
    Serial1.begin(1200);
    Setup_All_Pins();
    Setup_LCD();
    Setup_Perimeter();
    Setup_FSM();
}

void loop() {
    Loop_FSM();
}
