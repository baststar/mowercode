#pragma once

#include "../.pio/libdeps/megaatmega2560/Ardumower/code/tests/perimeterV2/perimeter.h"

extern Perimeter perimeter;
extern bool mowerIsInsideWire;

void Setup_Perimeter();
bool IsBounderyWireActive();
bool MowerIsInsideWire();
int GetCurrentMagnitude();
void UpdatePerimeterStatus();
