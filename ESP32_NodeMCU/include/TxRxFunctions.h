#pragma once

void ReceiveSerialData();
void ProcessData(int topic, int value);
void Setup_HardwareSerial();
void HandleNewData();
