#include "../.pio/libdeps/megaatmega2560/Ardumower/code/tests/perimeterV2/perimeter.h"
#include "../.pio/libdeps/megaatmega2560/Ardumower/code/tests/perimeterV2/adcman.h"
#include <Perimeter.h>
#include <config.h>

Perimeter perimeter;
unsigned long nextTimeUpdatePerimeterStatus = 0;
bool mowerIsInsideWire = false;
int currentMagnitude = 0;

void Setup_Perimeter() {
    ADCMan.init();
    perimeter.setPins(PIN_PERIMETER_LEFT, PIN_PERIMETER_RIGHT);
    perimeter.useDifferentialPerimeterSignal = true;
    perimeter.speedTest();
    ADCMan.run();
}

bool IsBounderyWireActive() {
    UpdatePerimeterStatus();
    if (currentMagnitude > -20 && currentMagnitude < 20) {
        return false;
    }
    return true;
}

bool MowerIsInsideWire() {
    return mowerIsInsideWire;
}

int GetCurrentMagnitude() {
    return currentMagnitude;
}

void UpdatePerimeterStatus() {
    if (millis() >= nextTimeUpdatePerimeterStatus) {
        nextTimeUpdatePerimeterStatus = millis() + 50;
        ADCMan.run();
        currentMagnitude = perimeter.getMagnitude(0);
        mowerIsInsideWire = perimeter.isInside(0);
    }
}
