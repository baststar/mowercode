#pragma once

void UpdateVoltAmpCharge();
void UpdateVoltAmpRain();
float GetAmps();
float GetBatteryVolt();
bool IsRaining();
bool IsCharging();
int GetRawRaining();
void SetVolt(int volt);
void SetAmp(int amp);
void SetRain(int rain);
