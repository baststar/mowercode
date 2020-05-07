#pragma once

#include <DS1302.h>

extern DS1302 rtc;

void SetTime(Time time);
char * GetTime();
