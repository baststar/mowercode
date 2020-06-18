#include <Arduino.h>
#include <Clock.h>
#include <Compass.h>
#include <EEPROMVariables.h>
#include <LCD.h>
#include <MemoryFree.h>
#include <MqttFunctions.h>
#include <NodeMCU.h>
#include <Perimeter.h>
#include <SetupPins.h>
#include <States/FSMMower.h>
#include <TxRxFunctions.h>
#include <VoltAmpRain.h>
#include <WLan.h>
#include <main.h>


TaskHandle_t Core0TaskHnd;
TaskHandle_t Core1TaskHnd;
SemaphoreHandle_t xSemaphore;

void setup() {

    Serial.begin(9600);

    // xSemaphore = xSemaphoreCreateBinary();

    Setup_All_Pins();
    Setup_LCD();
    // SetupRTC();
    // SetupCompass();
    // SetupVariablesFromEEPROM();
    // Setup_NodeMCU();
    // PrintEEPROM();

    Setup_Wlan();
    Setup_Mqtt();
    Setup_HardwareSerial();
    Serial.println("Setup complete!");

    // xTaskCreatePinnedToCore(Core1Task, "CPU_1", 10000, NULL, 1, &Core1TaskHnd, 1);
    // delay(500);
    // xTaskCreatePinnedToCore(Core0Task, "CPU_0", 1000, NULL, 1, &Core0TaskHnd, 0);
    // delay(500);
    // vTaskStartScheduler();
}

// void Core1Task(void *parameter) {
//     for (;;) {
//         xSemaphoreTake(xSemaphore, portMAX_DELAY);
//         Loop_FSM();
//         xSemaphoreGive(xSemaphore);
//         delay(50);
//     }
// }

// void Core0Task(void *parameter) {
//     for (;;) {
//         xSemaphoreTake(xSemaphore, portMAX_DELAY);
//         xSemaphoreGive(xSemaphore);
//         delay(50);
//     }
// }

void loop() {
    Loop_FSM();
    ReceiveSerialData();
}
