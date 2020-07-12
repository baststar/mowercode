#include <Arduino.h>
#include <States/FSMMower.h>
#include <VoltAmpRain.h>
#include <config.h>
#include <driver/adc.h>
#include <driver/gpio.h>

int RawRaining;
int Rain_Detected;

float Volts;
float Volts_Last;
int Zero_Volts;
float vout = 0.0;
float R1 = 330000;
float R2 = 22000;

int Sensor = A0; // Der Stromstärkesensor wird am Pin A0 (Analog "0") angeschlossen.
int VpA = 185;   // Millivolt pro Ampere (100 für 20A Modul und 66 für 30A Modul)
int sensorwert = 0;
int Nullpunkt = 717; // Spannung in mV bei dem keine Stromstärke vorhanden ist
double SensorSpannung = 0;
double Ampere = 0;

bool raining = false;

unsigned long currentMillis = 0;

unsigned long lastMillisVolt = 0;
unsigned long sendVoltInterval = 25;
const unsigned int bufferVoltageLength = 25;
unsigned int currentBufferVoltageMesurement = 0;
int voltages[bufferVoltageLength];

unsigned long lastMillisAmp = 0;
unsigned long sendAmpInterval = 1000;

unsigned long lastMillisRain = 0;
unsigned long sendRainInterval = 1000;

void Setup_VoltAmpRain() {
    adc1_config_width(ADC_WIDTH_BIT_13);
    adc1_config_channel_atten(PIN_VOLT_SENSOR, ADC_ATTEN_DB_0);
    adc1_config_channel_atten(PIN_AMP_SENSOR, ADC_ATTEN_DB_6);
    adc1_config_channel_atten(PIN_RAIN_SENSOR, ADC_ATTEN_DB_11);
}

void UpdateVoltAmpRain() {

    currentMillis = millis();
    if (currentMillis - lastMillisVolt > sendVoltInterval) {
        int read_raw = adc1_get_raw(PIN_VOLT_SENSOR);
        if (currentBufferVoltageMesurement < bufferVoltageLength) {
            voltages[currentBufferVoltageMesurement] = read_raw;
            currentBufferVoltageMesurement++;
        } else {
            currentBufferVoltageMesurement = 0;
            unsigned long int result = 0;
            for (int i = 0; i < bufferVoltageLength; i++) {
                result += voltages[i];
            }
            SetVolt(result / bufferVoltageLength);
        }
        lastMillisVolt = currentMillis;
    }

    if (currentMillis - lastMillisAmp > sendAmpInterval) {
        int rawVal = 0;
        for (int i = 0; i < 10; i++) {
            rawVal += adc1_get_raw(PIN_AMP_SENSOR) - 4096;
        }
        SetAmp(float(rawVal) / 10);
        lastMillisAmp = currentMillis;
    }

    if (currentMillis - lastMillisRain > sendRainInterval) {
        int rawVal = 0;
        for (int i = 0; i < 10; i++) {
            rawVal += adc1_get_raw(PIN_RAIN_SENSOR);
        }
        SetRain(float(rawVal) / 10);
        lastMillisRain = currentMillis;
    }
}

void SetVolt(int volt) {
    if (volt > 100) {
        vout = volt / (8191.0 * 1.20); // see text
        Volts = vout / (R2 / (R1 + R2));
        Volts_Last = Volts;
        Zero_Volts = 0;
    } else {
        Volts = Volts_Last;
        if (Zero_Volts > 5) {
            Volts = 0;
        }
    }
}

void SetAmp(int amp) {
    SensorSpannung = (amp / 8191.0) * 2000;        // Hier wird der Messwert in den Spannungswert am Sensor umgewandelt.
    Ampere = ((SensorSpannung - Nullpunkt) / VpA); // Im zweiten Schritt wird hier die Stromstärke berechnet.
}

void SetRain(int rain) {
    RawRaining = rain;
    if (rain < 2000) {
        raining = true;
    } else {
        raining = false;
    }
}

bool IsCharging() {
    return Ampere > 0.4;
}

float GetAmps() {
    return Ampere;
}

float GetBatteryVolt() {
    return Volts;
}

bool IsRaining() {
    return raining;
}
int GetRawRaining() {
    return RawRaining;
}
