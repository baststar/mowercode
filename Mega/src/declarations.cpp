
#include "../.pio/libdeps/megaatmega2560/Ardumower/code/tests/perimeterV2/perimeter.h"
#include <Arduino.h>
#include <DFRobot_QMC5883.h>
#include <DS1302.h>
#include <LiquidCrystal_I2C.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <TimeLib.h>
#include <Wire.h>
#include <config.h>
#include <stdio.h>

uint8_t lcd_Addr = LCD_ADDR;

LiquidCrystal_I2C lcd(lcd_Addr, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

String Version = "V6.6";

AlarmId id;

DFRobot_QMC5883 compass;

// Real Time Clock Pins
const int kCePin = PIN_REALTIME_CLOCK_RESET; // RST
const int kIoPin = PIN_REALTIME_CLOCK_DATA;  // DAT
const int kSclkPin = PIN_REALTIME_CLOCK_SCL; // CLK

DS1302 rtc(kCePin, kIoPin, kSclkPin);

// be careful that you really use PIN24.  The order is sometimes labelled
// so it looks like 24 is actually 22.

// Global Variables

// Perimeter Variables
Perimeter perimeter;
unsigned long nextTime = 0;
int counter = 0;
boolean inside = true;
int Wire_Detected;

int Loop_Cycle_Mowing = 0; // was byte before.

// Sonar Variables
// Time required for the sonar ping to be recieved by the echo pin.
long duration1 = 0;
long duration2 = 0;
long duration3 = 0;

int distance1 = 999; // Distance caculated  by the Sonar
int distance2 = 999;
int distance3 = 999;

int distance_blockage;

int Sonar_Hit_1_Total;
int Sonar_Hit_2_Total;
int Sonar_Hit_3_Total;
bool Sonar_Hit_Any_Total;
bool Sonar_Hit_1 = 0;
bool Sonar_Hit_2 = 0;
bool Sonar_Hit_3 = 0;
bool Sonar_Hit = 0;

// Bumper Variables
bool Bump_Frnt_LH;
bool Bump_Frnt_RH;
bool Bumper;

// Mower Status Variables
bool Mower_Docked;
bool Mower_Parked;
bool Mower_Running;
bool Mower_Parked_Low_Batt;
bool Mower_Error;
bool Manuel_Mode;

// Membrane Key Variables
byte Start_Key_X;
byte Plus_Key_X;
byte Minus_Key_X;
byte Stop_Key_X;
bool Menu_Complete_Settings;
bool Menu_Complete_Alarms;
bool Menu_Complete_Sensors;
bool Menu_Complete_Motion;
bool Menu_Complete_NAVI;
bool Menu_Complete_Tracking;
bool Menu_Complete;
byte Menu_Mode_Selection;
int Menu_View;
int Mow_Time_Set;
int Max_Options_Timing;
int Max_Options_Docked;
int Max_Options_Parked;
int Max_Options_Settings;
int Max_Options_Test;
int Max_Options_Alarms;
int Max_Options_Sensors;
int Max_Options_Motion;
int Max_Options_Tracking;
int Max_Options_NAVI;
int Max_Options_BETA;

// Serial Communication
float Volts;
float Volts_Last;
int Zero_Volts;
float Amps;
float VoltageAmp;
int RawValueAmp;
int RawValueVolt;
int Rain_Detected;
int Rain_Hit_Detected = 0;
int Charging;
// float Battery_Voltage_Last;
float Amps_Last;
int Volts_Outside_Reading;
byte OK_Nano_Data_Volt_Received;
byte OK_Nano_Data_Charge_Received;
byte Charge_Hits = 0;
byte Docked_Hits = 0;
bool Charge_Detected_MEGA = 0;

// Mow Calendar Variables
byte Alarm_Hour_Now;
byte Time_Hour;
byte Time_Minute;
byte Time_Second;
byte Time_Day;
byte Time_Month;
byte Time_Year;
byte Time_Date;
bool Alarm_Timed_Mow_ON = 0;
byte Alarm_Timed_Mow_Hour;   // Mowing Hour Number 3
byte Alarm_Timed_Mow_Minute; // Alarm minute 3

int Alarm_1_Saved_EEPROM;
int Alarm_2_Saved_EEPROM;
int Alarm_3_Saved_EEPROM;

String dayAsString(const Time::Day day)
{
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
    return "(unknown day)";
}

// Perimeter Wire Tracking
int I;
int Track_Wire_Itterations;
bool Outside_Wire;
byte Exit_Zone;
int MAG_Now;
int MAG_OUT_Stop;
int MAG_IN_Stop;
int MAG_TURN;
int MAG_Average_Start;
int MAG_Last;
byte Outside_Wire_Count = 0;
int Tracking_Wire = 0;
bool Wire_ON_Printed;
int Wire_Off;
int Wire_Refind_Tries = 0;

int Tracking_Turn_Left;
int Tracking_Turn_Right;
bool Mower_Track_To_Charge;
bool Mower_Track_To_Exit;

bool Abort_Wire_Find;
bool No_Wire_Found_Fwd;
bool No_Wire_Found_Bck;
int Wire_Find_Attempt = 0;

int PWM_Right;
int PWM_Left;
int MAG_Goal;
int MAG_Error;
int MAG_Start;
byte PWM_Blade_Speed_Min;
byte PWM_Blade_Speed_Max;
bool Blade_Override = 0;

// Compass Variables
float Compass_Heading_Degrees;
float Heading;
bool Compass_Heading_Locked = 0;
float Heading_Lock;
int Heading_Upper_Limit_Compass;
int Heading_Lower_Limit_Compass;
float Compass_Target;
int Compass_Leg = 0;
int Turn_Adjust = 0;
int error = 0;
float Calb_XAxis;
float Calb_YAxis;
float Calb_ZAxis;
int Tilt_X;
int Tilt_Y;
int Tilt_Z;
float X_Tilt;
float Y_Tilt;
float Z_Tilt;

// Pattern Mow
int Spiral_Mow = 1;
int Linking_Section;
int Leg = 1;
float Compass_Last;

// Wire Track Printer
int PrintInMax;
int PrintInMid;
int PrintInMin;
int PrintOutMin;
int PrintOutMid;
int PrintOutMax;
int PrintMAG_Now;

// WIFI Variables
float val_WIFI;

// EEPROM Saved Settings
int PWM_LEFT_EEPROM;
int PWM_RIGHT_EEPROM;
int PWM_BLADE_EEPROM;
int COMPASS_EEPROM;
float Tracking_PID_P_EEPROM;
int Pattern_Mow_EEPROM;
float Minimum_Volt_EEPROM;
int Compass_Home_EEPROM;
int Tip_Safety_EEPROM;
int Mower_Turn_Delay_Min_EEPROM;
int Mower_Turn_Delay_Max_EEPROM;
int Mower_Reverse_Delay_EEPROM;
bool Sonar_1_Activate_EEPROM;
bool Sonar_2_Activate_EEPROM;
bool Sonar_3_Activate_EEPROM;
int Track_Wire_Zone_1_Cycles_EEPROM;
int Track_Wire_Zone_2_Cycles_EEPROM;
bool Use_Charging_Station_EEPROM;
bool CW_Tracking_To_Charge_EEPROM;
bool CCW_Tracking_To_Charge_EEPROM;
bool CW_Tracking_To_Start_EEPROM;
bool CCW_Tracking_To_Start_EEPROM;
int Max_Cycles_Straight_EEPROM;
bool Compass_Heading_Hold_Enabled_EEPROM;
float CPower_EEPROM;
int Max_Sonar_Hit_EEPROM;
long maxdistancesonar_EEPROM;
bool Perimeter_Wire_Enabled_EEPROM;
int Max_Cycle_Wire_Find_EEPROM;
int Max_Cycle_Wire_Find_Back_EEPROM;
int Max_Tracking_Turn_Right_EEPROM;
int Max_Tracking_Turn_Left_EEPROM;
bool Rain_Sensor_Installed_EEPROM;
int Rain_Total_Hits_Go_Home_EEPROM;
bool WIFI_Enabled_EEPROM;
bool Cutting_Blades_Activate_EEPROM;
int Low_Battery_Instances_Chg_EEPROM;
int Alarm_1_Action_EEPROM;
bool Bumper_Activate_Frnt_EEPROM;

/***********************************************************************************************

                   SETUP OF MOWER

  The following setup parameters will setup the mower for your garden
  Turn on or off the settings to defien how you like the mower to behave.

  Settings marked with EEPROM can be adjusted using the mower LCD menu.  Once
changes and saved the settings will override the settings in this menu.  To
clear these settings you need to clear the EEPROM

  1 = Turned ON      0 = Turned OFF       Value = Value set for variable.

****************************************************************************************************/

// Activates the cutting blades and disc in the code
bool Cutting_Blades_Activate = CUTTING_BLADES_ACTIVATED;

bool Perimeter_Wire_Enabled = PERIMETER_WIRE_ENABLED; // Activates use of the perimeter
                                                      // boundary wire

bool WIFI_Enabled = WIFI_ENABLED; // Activates the WIFI Fucntions

// Docking Station
bool Use_Charging_Station = USE_CHARGING_STATION;                  // 1 if you are using the docking/charging
                                                                   // station, 0 if not
bool CW_Tracking_To_Charge = CLOCKWISE_TRACKING_TO_CHARGE;         // Clock-Wise tracking around the
                                                                   // boundary wire to the charging station
bool CCW_Tracking_To_Charge = COUNTERCLOCKWISE_TRACKING_TO_CHARGE; // Counter-Clock-Wise tracking around the
                                                                   // boundary wire to the charging station
bool CW_Tracking_To_Start = CLOCKWISE_TRACKING_TO_START;           // Clock-Wise  tracking around the boundary wire
                                                                   // when tracking to the start position
bool CCW_Tracking_To_Start = COUNTERCLOCKWISE_TRACKING_TO_START;   // Counter-Clock-Wise tracking around the
                                                                   // boundary wire to the charging station
byte Docked_Filter_Hits = DOCKED_FILTER_HITS;                      // Number of charge signals
                                                                   // to be detected before mower powers off

// Wire Tracking
int Track_Wire_Zone_1_Cycles = TRACK_WIRE_ZONE_1_CYCLES; // Zone 1 - Number of Itterations the
                                                         // PID function does before the
                                                         // mower exits the wire track
int Track_Wire_Zone_2_Cycles = TRACK_WIRE_ZONE_2_CYCLES; // Zone 2 - Therefore how long the mower is
                                                         // tracking the wire can be
                                                         // set = distance tracked.
int Max_Tracking_Turn_Right = MAX_TRACKING_TURN_RIGHT;   // The maximum number of turn right
                                                         // commands during wire tracking
                                                         // before a renewed wire find function
                                                         // is called (wheel spins)
int Max_Tracking_Turn_Left = MAX_TRACKING_TURN_LEFT;     // a re-find the wire sub-routine is
                                                         // called if this value is reached.
int Max_Cycle_Wire_Find = MAX_CYCLE_WIRE_FIND;           // Maximum number of forward tracking
                                                         // cycles in finding wire before the
                                                         // mower restarts a compass turn
                                                         // and wire find.
int Max_Cycle_Wire_Find_Back = MAX_CYCLE_WIRE_BACK;      // Maximum number of Backward tracking
                                                         // cycles in finding wire before the
                                                         // mower restarts a compass turn and
                                                         // wire find.

// Compass Settings
bool Compass_Activate = COMPASS_ENABLED;                          // Turns on the Compass (needs to be 1 to activate
                                                                  // further compass features)
bool Compass_Heading_Hold_Enabled = COMPASS_HEADING_HOLD_ENABLED; // Activates the compass heading hold
                                                                  // function to keep the mower straight
int Home_Wire_Compass_Heading = HOME_WIRE_COMPASS_HEADING;        // Heading the Mower will search for the
                                                                  // wire once the mowing is completed.
int CPower = COMPASS_CORRECTION_POWER; // Magnification of heading to PWM - How strong the mower
                                       // corrects itself in Compass Mowing

// Pattern Mow
int Pattern_Mow = PATTERN_MOW;                       // EEPROM
                                                     // 0 = OFF |  1 = Parallel (not working!!)
                                                     // | 3 = Sprials |
int Max_Cycles_Spirals = MAX_CYCLES_SPIRALS;         // Overrides the Max_Cycles for straught line
                                                     // mowing as the spirals
                                                     // requires more loops to complete
float Compass_Mow_Direction = COMPASS_MOW_DIRECTION; // Mow Direction of line when pattern
                                                     // mow is activated

// Safety Tilt Feature
int Tip_Safety = TIP_SAFETY_ENABLED; // EEPROM      // Experimental and not working yet!! - Stops
                                     // all motors if the mower overturns or is picked up.
int Max_Tilt = TIP_SAFETY_MAX_TILT;
int Min_Tilt = TIP_SAFETY_MIN_TILT;
int Max_Tilt_Hits = TIP_SAFETY_MAX_TILT_HITS;

// Compass alternative settings if the QMC Compass is freezing
int Compass_QMC_Refresh_Setting = COMPASS_QMC_REFRESH_SETTING; // 1 = 200HZ and 2 = 100Hz and
                                                               // 3 = 50HZ (Standard) 4 = 10Hz if
                                                               // compass is freezing try a different
                                                               // refresh rate.
int Compass_QMC_Sample_Setting = COMPASS_QMC_SAMPLE_SETTING;   // Number of samples per call to the Compass
                                                               // | 1 = 2 | 2 = 4 | 3 = 8 (Standard) |.

// Rain sensor
bool Rain_Sensor_Installed = RAIN_SENSOR_ENABLED;       // 1  = Rain sensor installed
                                                        // 0 = no sensor installed.
byte Rain_Total_Hits_Go_Home = RAIN_TOTAL_HITS_GO_HOME; // This sensor only makes
                                                        // sense in combination with a mower docking
                                                        // station as the mower is sent there to
                                                        // get out of the rain.

// Battery Settings
float Battery_Max = BATTERY_MAX;                               // Max battery volts in Volts. 3S = 12.6V
float Battery_Min = BATTERY_MIN;                               // EEPROM          // Lower Limit of battery charge
                                                               // before re-charge required.
byte Low_Battery_Detected = 0;                                 // Always set to 0
byte Low_Battery_Instances_Chg = LOW_BATTERY_INSTANCES_CHARGE; // Instances of low battery detected
                                                               // before a re-charge is called..

// Sonar Modules
bool Sonar_1_Activate = SONAR_ONE_ENABLED;   // Activate (1) Deactivate (0) Sonar 1
bool Sonar_2_Activate = SONAR_TWO_ENABLED;   // Activate (1) Deactivate (0) Sonar 2
bool Sonar_3_Activate = SONAR_THREE_ENABLED; // Activate (1) Deactivate (0) Sonar 3
int Max_Sonar_Hit = MAX_SONAR_HIT;           // Maximum number of Sonar hits before object is discovered
long maxdistancesonar = MAX_DISTANCE_SONAR;  // distance in cm from the mower that the sonar will activate at.

// Bumper Module
bool Bumper_Activate_Frnt =
    BUMPER_ACTIVATE_FRNT; // EEPROM // Activates the bumper bar on the front facia - defualt is off.  Enable in the LCD settings menu.

// Wheel Motors Setup
int Max_Cycles_Straight = MAX_CYCLES_STRAIGHT; // Number of loops the Sketch will run before the mower just turns
                                               // around anyway. Adjust according to your garden length
int PWM_MaxSpeed_LH = PWM_MAX_SPEED_LEFT;      // EEPROM            // Straight line speed LH Wheel (Looking from back
                                               // of mower)  Will be overidden if saved in EEPROM
int PWM_MaxSpeed_RH = PWM_MAX_SPEED_RIGHT;     // EEPROM            // Straight line speed RH Wheel - adjust to keep
                                               // mower tracking straight.  Will be overridden if saved in EEPROM

int Max_Motor_PWM_LH = MAX_MOTOR_PWM_LEFT;
int Max_Motor_PWM_RH = MAX_MOTOR_PWM_RIGHT;
int Mower_Turn_Delay_Min = MOWER_TURN_DELAY_MIN; // Min Max Turn time of the Mower after it reverses at the wire.
int Mower_Turn_Delay_Max = MOWER_TURN_DELAY_MAX; // A random turn time between these numbers is
                                                 // selected by the software
int Mower_Reverse_Delay = MOWER_REVERSE_DELAY;   // Time the mower reverses before making a turn.

// Blade Motor Setup
// Blade Speed can be modified in the settings menu and will be written to
// EEPROM The number below will then be overidden
byte PWM_Blade_Speed = PWM_BLADE_SPEED; // EEPROM
                                        // PWM signal sent to the blade motor (speed of
                                        // blade) new motor works well at 245.

// Alarm Setup
bool Set_Time = SET_TIME; // Turn to 1 to set time on RTC (Set time in Time tab
                          // Set_Time_On_RTC)
                          // After setting time turn to 0 and reload sketch.

// If the Alarm is changed in settings it will be written to EEPROM and the
// settings below will be overriden. Action for Alarm 1 is set to exit the dock
// and mow at this time. To change this action go to "void Activate_Alarms()"
bool Alarm_1_ON = ALARM_1_ON;         // EEPROM            // Activate Alarm 1  (1 = ON 0 = OFF)
int Alarm_1_Hour = ALARM_1_HOUR;      // EEPROM            // Mowing Hour Number 1
int Alarm_1_Minute = ALARM_1_MINUTE;  // EEPROM            // Alarm Minute 1
bool Alarm_1_Repeat = ALARM_1_REPEAT; // Repeat the Alarm at the same time
int Alarm_1_Action = ALARM_1_ACTION;  // EEPROM // Sets the actions to be performed when the alarm is called

// Action for Alarm 2 can be set in "void Activate_Alarms()"
bool Alarm_2_ON = ALARM_2_ON;         // EEPROM            // Activate Alarm 2 (1 = ON 0 = OFF)
int Alarm_2_Hour = ALARM_2_HOUR;      // EEPROM            // Mowing Hour Number 2
int Alarm_2_Minute = ALARM_2_MINUTE;  // EEPROM            // Alarm minute 2
bool Alarm_2_Repeat = ALARM_2_REPEAT; // Repeat the Alarm at the same time
int Alarm_2_Action = ALARM_2_ACTION;  // EEPROM // Sets the actions to be performed when the alarm is called

// Action for Alarm 3 can be set in "void Activate_Alarms()"
// Go Home Alarm
bool Alarm_3_ON = ALARM_3_ON;         // EEPROM            // Activate Alarm 3 (1 = ON 0 = OFF)
int Alarm_3_Hour = ALARM_3_HOUR;      // EEPROM            // Mowing Hour Number 3
int Alarm_3_Minute = ALARM_3_MINUTE;  // EEPROM            // Alarm minute 3
bool Alarm_3_Repeat = ALARM_3_REPEAT; // Repeat the Alarm at the same time
int Alarm_3_Action = ALARM_3_ACTION;  // EEPROM // Sets the actions to be performed when the alarm is called

byte Alarm_Second = ALARM_SECOND; // Seconds

/* Description of how the below values are displayed in the Serial Monitor Print
   Out for the wire function (InMax)                   Wire = 0 (OutMax) |
   (InMid)           |           (OutMid)     |
       |--------|--------|------|------|--------|--------|
       |        |        |      |      |        |        |
                      (InMin)       (OutMin)
*/

// Wire detection Values
/*Negative Values for In*/ // These values are based on the signal received by
                           // the wire sensor for my perimeter loop
int InMin = IN_MIN;
int InMid = IN_MID;
int InMax = IN_MAX; // the maximum received signal value  the wire
/*General Setup PID numbers for wire tracking*/
float P = MULTIPLICATION_FACTOR;                       // EEPROM  // Multiplication factor to the error measured
                                                       // to the wire center.  if jerky movement when tracking reduce
                                                       // number
float D = DAMPING_VALUE;                               // Dampening value to avoid the mower snaking on the wire.
byte Scale = SERIAL_MONITOR_LINE_TRACKING_PRINT_SCALE; // Serial Monitor Line Tracking Print Scale

// These values set the scale for the wire print out in the serial monitor once
// tracking
int OutMin = OUT_MIN;
int OutMid = OUT_MID;
int OutMax = OUT_MAX;

int Outside_Wire_Count_Max = OUTSIDE_WIRE_COUNT_MAX;               // If the mower is outside the wire this many
                                                                   // times the mower is stopped
int Action_On_Over_Wire_Count_Max = ACTION_ON_OVER_WIRE_COUNT_MAX; // Set 1 to hibernate mower (Power Off
                                                                   // and Stop)   Set 2 to refind garden
                                                                   // using sonar and wire detect function
                                                                   // 3 to do a refind wire function

bool Show_TX_Data = SHOW_TX_DATA; // Show the values recieved from the Nano / ModeMCU
                                  // in the serial monitor

/************************************************************************************************************/
