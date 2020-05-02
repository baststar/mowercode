#pragma once

#include <Arduino.h>

void PrintWirePosition();
void Track_Wire_From_Dock_to_Zone_X(int zone);
void Tracking_Restart_Blocked_Path2();
int sFunction(double magnitude, double maxMagnitude, int maxSpeed);
void Track_Perimeter_Wire(int maxTimeInMilliseconds);