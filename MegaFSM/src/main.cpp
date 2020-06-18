#include <Arduino.h>
#include <Clock.h>
#include <Compass.h>
#include <EEPROMVariables.h>
#include <LCD.h>
#include <MemoryFree.h>
#include <NodeMCU.h>
#include <Perimeter.h>
#include <SetupPins.h>
#include <States/FSMMower.h>
#include <VoltAmpRain.h>
#include <NodeMCU.h>


void setup() {
    Serial.begin(9600);
    // Serial1.begin(1200);
    Setup_All_Pins();
    Setup_LCD();
    Setup_Perimeter();
    SetupRTC();
    SetupCompass();
    SetupVariablesFromEEPROM();
    Setup_NodeMCU();
    // PrintEEPROM();
    Serial.println("FREEMEM: " + String(freeMemory()));
}

void loop() {
    Loop_FSM();
}
