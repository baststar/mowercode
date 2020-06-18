#include <Arduino.h>
#include <NodeMCU.h>
#include <config.h>

void Setup_NodeMCU() {
    if (USE_NODEMCU == 1) {
        Serial2.begin(9600); // If WIFI is on open Serial port 2 for the NodeMCU communication
    }
}
#define INPUT_SIZE 30
void SendData(NodeMCUMessageTopics topic, String value) {
    Serial2.print(String(topic) + "=" + value + ";");
}
