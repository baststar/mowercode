#include "../.pio/libdeps/megaatmega2560/Ardumower/code/tests/perimeterV2/perimeter.h"
#include "../.pio/libdeps/megaatmega2560/Ardumower/code/tests/perimeterV2/adcman.h"
#include <Perimeter.h>
#include <config.h>

Perimeter perimeter;
long int nextTimeUpdatePerimeterStatus = 0;
bool mowerIsInsideWire = false;
int currentMagnitude = 0;
long int currMillis = 0;

void Setup_Perimeter() {
    ADCMan.init();
    perimeter.setPins(PIN_PERIMETER_LEFT, PIN_PERIMETER_RIGHT);
    perimeter.useDifferentialPerimeterSignal = true;
    perimeter.speedTest();
    ADCMan.run();
}

bool IsBounderyWireActive() {
    if (currentMagnitude < -50 || currentMagnitude > 50) {
        return true;
    }
    return false;
}

bool MowerIsInsideWire() {
    return mowerIsInsideWire;
}

int GetCurrentMagnitude() {
    return currentMagnitude;
}

void UpdatePerimeterStatus() {
    currMillis = millis();
    if (currMillis >= nextTimeUpdatePerimeterStatus) {
        ADCMan.run();
        nextTimeUpdatePerimeterStatus = currMillis + 25;
        currentMagnitude = perimeter.getMagnitude(0);
        mowerIsInsideWire = perimeter.isInside(0);
    }
}
