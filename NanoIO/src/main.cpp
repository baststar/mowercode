#include <ACS712_AmpSensor.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <adcman.h>
#include <config.h>
#include <main.h>
#include <perimeter.h>


Perimeter perimeter;
SoftwareSerial serialToESP32(PIN_RX, PIN_TX);

unsigned long currentMillis = 0;
unsigned long lastMillisPerimeter = 0;
unsigned long sendPerimeterInterval = 50;
int isInside = -1;

void setup() {
    Serial.begin(9600);
    serialToESP32.begin(9600);
    ADCMan.init();
    // ADCMan.sampleRate = SRATE_9615;
    perimeter.setPins(PIN_PERIMETER_LEFT, PIN_PERIMETER_RIGHT);
    perimeter.speedTest();
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    delay(25);
    Serial.println("setup done!");
}


void loop() {

    currentMillis = millis();
    ADCMan.run();

    if (currentMillis - lastMillisPerimeter > sendPerimeterInterval) {

        if (ADCMan.isCaptureComplete(PIN_PERIMETER_LEFT)) {

            char message[10];
            snprintf(message, sizeof(message), "%d", perimeter.getMagnitude(0));
            // Serial.println(message);
            // SendDataToESP32(NodeMCUMessageTopics::PerimeterValue, message);

            int inside = perimeter.isInside(0) == true ? 1 : 0;

            if (isInside != inside) {
                snprintf(message, sizeof(message), "%d", inside);
                SendDataToESP32(NodeMCUMessageTopics::PerimeterInsideValue, message);
                isInside = inside;
                // Serial.println("send");
                digitalWrite(LED_BUILTIN, isInside == 1 ? 0 : 1);
            }

            lastMillisPerimeter = currentMillis;
        }
    }
}

void SendDataToESP32(NodeMCUMessageTopics topic, char *value) {
    serialToESP32.print('|' + String(topic) + "=" + value + ";" + '\n');
}
