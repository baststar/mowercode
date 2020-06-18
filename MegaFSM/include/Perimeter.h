#pragma once

#include "../.pio/libdeps/megaatmega2560/Ardumower/code/ardumower/perimeter.h"

extern Perimeter perimeter;
extern bool mowerIsInsideWire;

void Setup_Perimeter();
bool IsBounderyWireActive();
bool MowerIsInsideWire();
int GetCurrentMagnitudeRight();
int GetCurrentMagnitudeLeft();
void UpdatePerimeterStatus();
