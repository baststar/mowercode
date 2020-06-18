
#include <Arduino.h>
#include <Clock.h>
#include <CustomFunctions.h>
#include <EEPROMVariables.h>
#include <LCD.h>
#include <RtcDS3231.h>
#include <States/FSMMower.h>
#include <States/FSMStates.h>
#include <config.h>
#include <States/FSMMower.h>


RtcDS3231<TwoWire> Rtc(Wire);

void SetupRTC() {

    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

    if (!Rtc.IsDateTimeValid()) {

        if (Rtc.LastError() != 0) {
            // we have a communications error
            // see https://www.arduino.cc/en/Reference/WireEndTransmission for
            // what the number means
            Serial.print("RTC communications error = ");
            Serial.println(Rtc.LastError());
            ShowError("RTC error " + String(Rtc.LastError()));

        } else {

            // Common Causes:
            //    1) first time you ran and the device wasn't running yet
            //    2) the battery on the device is low or even missing

            Serial.println("RTC lost confidence in the DateTime!");

            // following line sets the RTC to the date & time this sketch was compiled
            // it will also reset the valid flag internally unless the Rtc device is
            // having an issue

            Rtc.SetDateTime(compiled);
        }
    }

    if (!Rtc.GetIsRunning()) {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();

    if (now < compiled) {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    } else if (now > compiled) {
        Serial.println("RTC is newer than compile time. (this is expected)");
    } else if (now == compiled) {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }

    // never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);

    String dateString = GetDateTimeAsString();

    Serial.println("RTC: " + dateString);
}

String GetDateTimeAsString() {
    RtcDateTime now = Rtc.GetDateTime();
    char datestring[25];
    snprintf_P(datestring, ARRAY_SIZE(datestring), PSTR("%02u/%02u/%04u %02u:%02u"), now.Month(), now.Day(), now.Year(), now.Hour(), now.Minute());
    return String(datestring);
}

String GetTimeAsString() {
    RtcDateTime now = Rtc.GetDateTime();
    char datestring[25];
    snprintf_P(datestring, ARRAY_SIZE(datestring), PSTR("%02u:%02u:%02u"), now.Hour(), now.Minute(), now.Second());
    return String(datestring);
}

String GetTemperature() {
    RtcTemperature temp = Rtc.GetTemperature();
    return String(temp.AsFloatDegC());
}

void TestRTC() {

    if (!Rtc.IsDateTimeValid()) {

        if (Rtc.LastError() != 0) {

            // we have a communications error
            // see https://www.arduino.cc/en/Reference/WireEndTransmission for
            // what the number means
            Serial.print("RTC communications error = ");
            Serial.println(Rtc.LastError());
            ShowError("RTC error " + String(Rtc.LastError()));

        } else {

            // Common Causes:
            //    1) the battery on the device is low or even missing and the power line was disconnected
            Serial.println("RTC lost confidence in the DateTime!");
            ShowError("RTC lost confidence");
        }
    }
}

unsigned long currentMillies = 0;
unsigned long lastChecked = 0;
unsigned long checkInterval = 60000;

void CheckAlerts() {
    if (currentState == STATE_DOCKED) {
        currentMillies = currentMillisGlobal;
        if (currentMillies - lastChecked > checkInterval) {
            lastChecked = currentMillies;
            RtcDateTime now = Rtc.GetDateTime();
            if (currentState == STATE_DOCKED) {
                // Check Alert 1
                if (eeprom_alarm_hour_1 == now.Hour() && eeprom_alarm_minute_1 == now.Minute()) {
                    StartSequence(eeprom_alarm_sequence_1);
                } else if (eeprom_alarm_hour_2 == now.Hour() && eeprom_alarm_minute_2 == now.Minute()) {
                    StartSequence(eeprom_alarm_sequence_2);
                } else if (eeprom_alarm_hour_3 == now.Hour() && eeprom_alarm_minute_3 == now.Minute()) {
                    StartSequence(eeprom_alarm_sequence_3);
                }
            }
        }
    }
}
