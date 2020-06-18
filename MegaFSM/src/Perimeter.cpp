#include "../.pio/libdeps/megaatmega2560/Ardumower/code/ardumower/perimeter.h"
#include "../.pio/libdeps/megaatmega2560/Ardumower/code/ardumower/adcman.h"
#include <EEPROMex.h>
#include <Perimeter.h>
#include <States/FSMMower.h>
#include <config.h>


Perimeter perimeter;
unsigned long nextTimeUpdatePerimeterStatus = 0;
bool mowerIsInsideWire = false;
int currentMagnitudeRight = 0;
int currentMagnitudeLeft = 0;

void Setup_Perimeter() {
    // EEPROM.write(500, 0);
    // ADCMan.sampleRate = SRATE_9615;
    ADCMan.init();

    pinMode(VOLT_PIN, INPUT);
    pinMode(AMP_PIN, INPUT);
    pinMode(RAIN_PIN, INPUT);
    ADCMan.setCapture(VOLT_PIN, 1, 0);
    ADCMan.setCapture(AMP_PIN, 1, 0);
    ADCMan.setCapture(RAIN_PIN, 1, 0);
    perimeter.setPins(PIN_PERIMETER_LEFT, PIN_PERIMETER_RIGHT);
    perimeter.speedTest();
    // ADCMan.calibrate();
    ADCMan.run();
}

bool IsBounderyWireActive() {
    if (currentMagnitudeLeft < -50 || currentMagnitudeLeft > 50) {
        return true;
    }
    return false;
}

bool MowerIsInsideWire() {
    return mowerIsInsideWire;
}

int GetCurrentMagnitudeRight() {
    return currentMagnitudeRight;
}

int GetCurrentMagnitudeLeft() {
    return currentMagnitudeLeft;
}

void UpdatePerimeterStatus() {
    if (currentMillisGlobal >= nextTimeUpdatePerimeterStatus) {
        ADCMan.run();
        nextTimeUpdatePerimeterStatus = currentMillisGlobal + 50;
        currentMagnitudeLeft = perimeter.getMagnitude(0);
        currentMagnitudeRight = perimeter.getMagnitude(1);
        mowerIsInsideWire = perimeter.isInside(1);
    }
}
