#include <Arduino.h>
#include <Clock.h>
#include <Compass.h>
#include <EEPROMVariables.h>
#include <LCD.h>
#include <MemoryFree.h>
#include <Perimeter.h>
#include <SetupPins.h>
#include <States/FSMMower.h>



void setup() {
    Serial.begin(115200);
    Serial1.begin(1200);
    Setup_All_Pins();
    Setup_LCD();
    Setup_Perimeter();
    SetupRTC();
    SetupCompass();
    SetupVariablesFromEEPROM();
    // PrintEEPROM();
    Serial.println("FREE: " + String(freeMemory()));
}

void loop() {
    Loop_FSM();
}
