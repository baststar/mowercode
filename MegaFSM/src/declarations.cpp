
#include <Arduino.h>
#include <DFRobot_QMC5883.h>
#include <DS1302.h>
#include <LiquidCrystal_I2C.h>
#include <config.h>
#include <declarations.h>

DFRobot_QMC5883 compass;
DS1302 rtc(PIN_REALTIME_CLOCK_RESET, PIN_REALTIME_CLOCK_DATA, PIN_REALTIME_CLOCK_SCL);
