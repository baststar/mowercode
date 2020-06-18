#include "../.pio/libdeps/megaatmega2560/Ardumower/code/ardumower/adcman.h"
#include <Arduino.h>
#include <States/FSMMower.h>
#include <VoltAmpRain.h>
#include <config.h>


int RawAmpere = 0;
int RawVolt = 0;
int RawRaining = 0;

unsigned long lastCheckedVoltAmpRain = 0;


void UpdateVoltAmpRain() {

    if (currentMillisGlobal - lastCheckedVoltAmpRain > 50) {

        lastCheckedVoltAmpRain = currentMillisGlobal;

        ADCMan.run();
        if (ADCMan.isCaptureComplete(AMP_PIN)) {
            RawAmpere = ADCMan.read(AMP_PIN);
        }

        if (ADCMan.isCaptureComplete(VOLT_PIN)) {
            RawVolt = ADCMan.read(VOLT_PIN);
        }

        if (ADCMan.isCaptureComplete(RAIN_PIN)) {
            RawRaining = ADCMan.read(RAIN_PIN);
        }

        UpdateVoltAmpCharge();
    }
}

int Rain_Detected;
float Volts;
float Volts_Last;
int Zero_Volts;
float vout = 0.0;
float R1 = 30000; // Mower 330 = 3000    Mower LAM = 30000
float R2 = 7500;  // Mower 330 = 7000

int Sensor = A0; // Der Stromstärkesensor wird am Pin A0 (Analog "0") angeschlossen.
int VpA = 185;   // Millivolt pro Ampere (100 für 20A Modul und 66 für 30A Modul)
int sensorwert = 0;
int Nullpunkt = 1600; // Spannung in mV bei dem keine Stromstärke vorhanden ist
double SensorSpannung = 0;
double Ampere = 0;

bool raining = false;

void UpdateVoltAmpCharge() {

    SensorSpannung = (RawAmpere / 1024.0) * 5000;  // Hier wird der Messwert in den Spannungswert am Sensor umgewandelt.
    Ampere = ((SensorSpannung - Nullpunkt) / VpA); // Im zweiten Schritt wird hier die Stromstärke berechnet.

    if (RawVolt > 100) {

        vout = (RawVolt * 5.0) / 1024.0; // see text
        Volts = vout / (R2 / (R1 + R2));
        Volts_Last = Volts;
        Zero_Volts = 0;

    } else {
        Volts = Volts_Last;
        Zero_Volts = Zero_Volts + 1;
        if (Zero_Volts > 5) {
            Volts = 0;
        }
    }

    if (RawRaining < 150) {
        raining = true;
    } else {
        raining = false;
    }
}

bool IsCharging() {
    return Ampere > 1.1;
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