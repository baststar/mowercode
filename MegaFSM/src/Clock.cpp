
#include <Arduino.h>
#include <DS1302.h>
#include <config.h>
#include <string.h>

DS1302 rtc(PIN_REALTIME_CLOCK_RESET, PIN_REALTIME_CLOCK_DATA, PIN_REALTIME_CLOCK_SCL);

String dayAsString(const Time::Day day) {
    switch (day) {
    case Time::kSunday:
        return "Sunday";
    case Time::kMonday:
        return "Monday";
    case Time::kTuesday:
        return "Tuesday";
    case Time::kWednesday:
        return "Wednesday";
    case Time::kThursday:
        return "Thursday";
    case Time::kFriday:
        return "Friday";
    case Time::kSaturday:
        return "Saturday";
    }
    return "nd";
}

void SetTime(Time time) {
    // Set_Time to 1 in the setting menu to set time.  Load the sketch then immediatley Set_Time = 0 and reload the sketch.
    rtc.writeProtect(false);
    rtc.halt(false);
    rtc.time(time);
    // rtc.writeProtect(true);
}

char * GetTime() {
    Time t = rtc.time();
    const String day = dayAsString(t.day);
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "%s %04d-%02d-%02d %02d:%02d:%02d", day.c_str(), t.yr, t.mon, t.date, t.hr, t.min, t.sec);
    Serial.println(buffer);
    return buffer;
}
