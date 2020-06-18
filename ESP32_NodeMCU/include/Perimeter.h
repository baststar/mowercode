#pragma once

extern bool mowerIsInsideWire;

bool IsBounderyWireActive();
bool MowerIsInsideWire();
int GetCurrentMagnitudeRight();
int GetCurrentMagnitudeLeft();
void SetMagnitudeLeft(int magnitude);
void SetInside(bool inside);
