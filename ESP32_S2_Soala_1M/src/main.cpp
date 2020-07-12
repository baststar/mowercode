#include <Arduino.h>
#include <Clock.h>
#include <Compass.h>
#include <EEPROMVariables.h>
#include <LCD.h>
#include <MemoryFree.h>
#include <Perimeter.h>
#include <SetupPins.h>
#include <States/FSMMower.h>
#include <TxRxFunctions.h>
#include <VoltAmpRain.h>
#include <WLan.h>
#include <main.h>

void setup() {

    Serial.begin(9600);

    Setup_All_Pins();
    Setup_LCD();
    SetupRTC();
    Setup_VoltAmpRain();
    SetupCompass();
    SetupVariablesFromEEPROM();
    PrintEEPROM();
    Setup_Wlan();
    Setup_HardwareSerial();
    Serial.println("Setup complete!");
}

void loop() {
    Loop_FSM();
}
