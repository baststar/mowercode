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
ACS712 sensor(ACS712_05B, AMP_PIN);

unsigned long currentMillis = 0;
unsigned long lastMillisPerimeter = 0;
unsigned long sendPerimeterInterval = 50;
int isInside = -1;

unsigned long lastMillisAmp = 0;
unsigned long sendAmpInterval = 500;

unsigned long lastMillisVolt = 0;
unsigned long sendVoltInterval = 3000;

unsigned long lastMillisRain = 0;
unsigned long sendRainInterval = 4000;

void setup() {
    Serial.begin(9600);
    serialToESP32.begin(115200);

    ADCMan.init();
    // ADCMan.sampleRate = SRATE_9615;
    perimeter.setPins(PIN_PERIMETER_LEFT, PIN_PERIMETER_RIGHT);
    perimeter.speedTest();
    ADCMan.setCapture(VOLT_PIN, 10, false);
    ADCMan.setCapture(RAIN_PIN, 10, false);
    ADCMan.setCapture(AMP_PIN, 10, false);

    // ADCMan.calibrate();

    // ADCMan.run();

    delay(25);

    // int zero = sensor.calibrate();
    // Serial.println("Amp-Sensor calibrated");
    Serial.println("setup done!");
}


void loop() {

    currentMillis = millis();
    ADCMan.run();

    if (currentMillis - lastMillisPerimeter > sendPerimeterInterval) {

        if (ADCMan.isCaptureComplete(PIN_PERIMETER_LEFT)) {

            char message[10];
            snprintf(message, sizeof(message), "%d", perimeter.getMagnitude(0));
            // SendDataToESP32(NodeMCUMessageTopics::PerimeterValue, message);

            int inside = perimeter.isInside(0) == true ? 1 : 0;

            if (isInside != inside) {
                snprintf(message, sizeof(message), "%d", inside);
                SendDataToESP32(NodeMCUMessageTopics::PerimeterInsideValue, message);
                isInside = inside;
            }

            lastMillisPerimeter = currentMillis;
        }
    }

    if (currentMillis - lastMillisAmp > sendAmpInterval) {
        if (ADCMan.isCaptureComplete(AMP_PIN)) {
            // int I = sensor.getCurrentDC() * 100;
            char message[10];
            snprintf(message, sizeof(message), "%d", ADCMan.read(AMP_PIN) - 512);
            SendDataToESP32(NodeMCUMessageTopics::AmpValue, message);
            lastMillisAmp = currentMillis;
            // Serial.println(ADCMan.read(AMP_PIN));
        }
    }

    if (currentMillis - lastMillisVolt > sendVoltInterval) {
        if (ADCMan.isCaptureComplete(VOLT_PIN)) {
            char message[10];
            snprintf(message, sizeof(message), "%d", ADCMan.read(VOLT_PIN));
            SendDataToESP32(NodeMCUMessageTopics::VoltValue, message);
            lastMillisVolt = currentMillis;
        }
    }

    if (currentMillis - lastMillisRain > sendRainInterval) {
        if (ADCMan.isCaptureComplete(RAIN_PIN)) {
            char message[10];
            snprintf(message, sizeof(message), "%d", ADCMan.read(RAIN_PIN));
            SendDataToESP32(NodeMCUMessageTopics::RainValue, message);
            lastMillisRain = currentMillis;
        }
    }
}

void SendDataToESP32(NodeMCUMessageTopics topic, char *value) {
    serialToESP32.print('|' + String(topic) + "=" + value + ";" + '\n');
    // Serial.print('|' + String(topic) + "=" + value + ";" + '\n');
    // Wire.beginTransmission(4);
    // char message[10];
    // snprintf(message, sizeof(message), "|%d=%s;", topic, value);
    // Wire.write(message);
    // Wire.endTransmission();
}
