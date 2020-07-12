#include <Arduino.h>
#include <CustomFunctions.h>
#include <Perimeter.h>
#include <TxRxFunctions.h>
#include <VoltAmpRain.h>
#include <WLan.h>
#include <Wire.h>
#include <config.h>


HardwareSerial serialNano(1);

enum NodeMCUMessageTopics { NewState = 1, PerimeterValue = 2, PerimeterInsideValue = 3, AmpValue = 4, VoltValue = 5, RainValue = 6 };

const byte numChars = 32;
char receivedChars[numChars];
bool newData = false;

void Setup_HardwareSerial() {
    serialNano.begin(9600, SERIAL_8N1, PIN_RX, PIN_TX, false);
}

void HandleNewData() {

    if (newData == true) {

        // ignore incomplete serialbuffer-content by checking the startcharacter '|'
        if (receivedChars[0] != '|') {
            newData = false;
        } else {

            // remove startcharacter '|'
            memmove(receivedChars, receivedChars + 1, strlen(receivedChars + 1) + 1);

            newData = false;

            char *command = strtok(receivedChars, ";");

            while (command != 0) {
                // Split the command in two values
                char *separator = strchr(command, '=');
                if (separator != 0) {
                    // Actually split the string in 2: replace ':' with 0
                    *separator = 0;
                    int topic = atoi(command);
                    ++separator;
                    int value = atoi(separator);

                    // Do something with servoId and position

                    ProcessData(topic, value);
                }
                // Find the next command in input string
                command = strtok(0, ";");
            }
        }
    }
}

unsigned long lastMillisReceiveData = 0;
unsigned long receiveDataInterval = 250;

void ReceiveSerialData() {

    if (millis() - lastMillisReceiveData > receiveDataInterval) {

        static byte ndx = 0;
        char endMarker = '\n';
        char rc;

        // Serial.println(Serial.available());

        while (serialNano.available() > 0 && newData == false) {

            rc = serialNano.read();

            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            } else {
                receivedChars[ndx] = '\0'; // terminate the string
                ndx = 0;
                newData = true;
            }
        }
        HandleNewData();
        lastMillisReceiveData = millis();
    }
}

String mowerStates[] = {"STATE_DOCKED",
                        "STATE_DOCKED_MENU",
                        "STATE_EXIT_GARAGE",
                        "STATE_MOWING",
                        "STATE_FOLLOW_WIRE",
                        "STATE_PARKED",
                        "STATE_PARKED_MENU",
                        "STATE_RANDOM_ROTATE",
                        "STATE_WIRE_TO_GARDEN",
                        "STATE_TEST_MENU",
                        "STATE_ERROR",
                        "STATE_COMPASS_ROTATE",
                        "STATE_ROTATE_TO_WIRE",
                        "STATE_FIND_WIRE_FORWARDS",
                        "STATE_FIND_WIRE_BACKWARDS",
                        "STATE_SETTINGS_MENU",
                        "STATE_SETTINGS_MOTORSPEED_MENU",
                        "STATE_SETTINGS_TIMES_MENU",
                        "STATE_SETTINGS_PERIMETER_MENU",
                        "STATE_SETTINGS_MOWTIMES_MENU",
                        "STATE_SETTINGS_ALARMS_MENU"};


void ProcessData(int topic, int value) {

    Serial.println("topic:" + String(topic) + " value " + String(value));

    switch (topic) {
    case NodeMCUMessageTopics::NewState:

        if (ARRAY_SIZE(mowerStates) >= value) {
            // mqttClient.publish("mower", mowerStates[value - 1], true, 3);
        } else {
            // mqttClient.publish("mower", "unkown State", true, 3);
        }
        break;

    case NodeMCUMessageTopics::PerimeterValue:
        SetMagnitudeLeft(value);
        // mqttClient.publish("mower", "Perimeter: " + String(value));
        break;
    case NodeMCUMessageTopics::PerimeterInsideValue:
        SetInside(value);
        // mqttClient.publish("mower", "Perimeter: " + String(value));
        break;
    case NodeMCUMessageTopics::AmpValue:
        SetAmp(value);
        // mqttClient.publish("mower", "Ampere: " + String(value));
        break;
    case NodeMCUMessageTopics::VoltValue:
        SetVolt(value);
        // mqttClient.publish("mower", "Volt: " + String(value));
        break;
    case NodeMCUMessageTopics::RainValue:
        SetRain(value);
        break;

    default:
        // Serial.println("default");
        // mqttClient.publish("mower", "default", true, 3);
        break;
    }
}
