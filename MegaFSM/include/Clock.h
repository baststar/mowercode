#pragma once

#include <RtcDS3231.h>
#include <Wire.h>

extern RtcDS3231<TwoWire> Rtc;

void SetupRTC();
void TestRTC();
String GetDateTimeAsString();
String GetTemperature();
