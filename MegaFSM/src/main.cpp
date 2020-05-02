#include <Arduino.h>
#include <LCD.h>
#include <Perimeter.h>
#include <SetupPins.h>
#include <States/FSMMower.h>


void setup() {
    Serial.begin(115200);
    Serial.println("SETUP");
    Setup_All_Pins();
    Setup_LCD();
    Setup_Perimeter();
    Setup_FSM();
}

void loop() {
    Loop_FSM();
}
