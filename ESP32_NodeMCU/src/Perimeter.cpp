
#include <Perimeter.h>
#include <States/FSMMower.h>
#include <config.h>


unsigned long nextTimeUpdatePerimeterStatus = 0;
bool mowerIsInsideWire = false;
int currentMagnitudeRight = 0;
int currentMagnitudeLeft = 0;


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

void SetMagnitudeLeft(int magnitude) {
    currentMagnitudeLeft = magnitude;
}

void SetInside(bool inside) {
    mowerIsInsideWire = inside;
}
