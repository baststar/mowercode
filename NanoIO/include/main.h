#pragma once

#include <Arduino.h>

enum NodeMCUMessageTopics { NewState = 1, PerimeterValue = 2, PerimeterInsideValue = 3, AmpValue = 4, VoltValue = 5, RainValue = 6 };

void SendDataToESP32(NodeMCUMessageTopics topic, char *value);
