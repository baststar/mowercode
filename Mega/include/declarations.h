#pragma once

#include "../.pio/libdeps/megaatmega2560/Ardumower/code/tests/perimeterV2/perimeter.h"
#include <Arduino.h>
#include <DFRobot_QMC5883.h>
#include <DS1302.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Libraries for Real Time Clock
#include <stdio.h>

extern LiquidCrystal_I2C lcd;
extern DS1302 rtc;
extern DFRobot_QMC5883 compass;

extern String dayAsString(const Time::Day day);

extern String Version;
extern bool WIFI_Enabled;
extern bool Set_Time;

// Perimeter Variables
extern Perimeter perimeter;
extern unsigned long nextTime;
extern int counter;
extern boolean inside;
extern int Wire_Detected;

extern int Loop_Cycle_Mowing; // was byte before.

// Sonar Variables
extern long duration1;
extern long duration2;
extern long duration3;

extern int distance1;
extern int distance2;
extern int distance3;

extern int distance_blockage;

extern int Sonar_Hit_1_Total;
extern int Sonar_Hit_2_Total;
extern int Sonar_Hit_3_Total;
extern bool Sonar_Hit_Any_Total;
extern bool Sonar_Hit_1;
extern bool Sonar_Hit_2;
extern bool Sonar_Hit_3;
extern bool Sonar_Hit;

// Bumper Variables
extern bool Bump_Frnt_LH;
extern bool Bump_Frnt_RH;
extern bool Bumper;

// Mower Status Variables
extern bool Mower_Docked;
extern bool Mower_Parked;
extern bool Mower_Running;
extern bool Mower_Parked_Low_Batt;
extern bool Mower_Error;
extern bool Manuel_Mode;

// Membrane Key Variables
extern byte Start_Key_X;
extern byte Plus_Key_X;
extern byte Minus_Key_X;
extern byte Stop_Key_X;

extern bool Menu_Complete_Settings;
extern bool Menu_Complete_Alarms;
extern bool Menu_Complete_Sensors;
extern bool Menu_Complete_Motion;
extern bool Menu_Complete_NAVI;
extern bool Menu_Complete_Tracking;
extern bool Menu_Complete;
extern byte Menu_Mode_Selection;
extern int Menu_View;
extern int Mow_Time_Set;
extern int Max_Options_Timing;
extern int Max_Options_Docked;
extern int Max_Options_Parked;
extern int Max_Options_Settings;
extern int Max_Options_Test;
extern int Max_Options_Alarms;
extern int Max_Options_Sensors;
extern int Max_Options_Motion;
extern int Max_Options_Tracking;
extern int Max_Options_NAVI;
extern int Max_Options_BETA;

// Serial Communication
extern float Volts;
extern float Volts_Last;
extern int Zero_Volts;
extern float Amps;
extern float VoltageAmp;
extern int RawValueAmp;
extern int RawValueVolt;
extern int Rain_Detected;
extern int Rain_Hit_Detected;
extern int Charging;

// float Battery_Voltage_Last;
extern float Amps_Last;
extern int Volts_Outside_Reading;
extern byte OK_Nano_Data_Volt_Received;
extern byte OK_Nano_Data_Charge_Received;
extern byte Charge_Hits;
extern byte Docked_Hits;
extern bool Charge_Detected_MEGA;

// Mow Calendar Variables
extern byte Alarm_Hour_Now;
extern byte Time_Hour;
extern byte Time_Minute;
extern byte Time_Second;
extern byte Time_Day;
extern byte Time_Month;
extern byte Time_Year;
extern byte Time_Date;
extern bool Alarm_Timed_Mow_ON;
extern byte Alarm_Timed_Mow_Hour;   // Mowing Hour Number 3
extern byte Alarm_Timed_Mow_Minute; // Alarm minute 3

extern int Alarm_1_Saved_EEPROM;
extern int Alarm_2_Saved_EEPROM;
extern int Alarm_3_Saved_EEPROM;

// Perimeter Wire Tracking
extern int I;
extern int Track_Wire_Itterations;
extern bool Outside_Wire;
extern byte Exit_Zone;
extern int MAG_Now;
extern int MAG_OUT_Stop;
extern int MAG_IN_Stop;
extern int MAG_TURN;
extern int MAG_Average_Start;
extern int MAG_Last;
extern byte Outside_Wire_Count;
extern int Tracking_Wire;
extern bool Wire_ON_Printed;
extern int Wire_Off;
extern int Wire_Refind_Tries;

extern int Tracking_Turn_Left;
extern int Tracking_Turn_Right;
extern bool Mower_Track_To_Charge;
extern bool Mower_Track_To_Exit;

extern bool Abort_Wire_Find;
extern bool No_Wire_Found_Fwd;
extern bool No_Wire_Found_Bck;
extern int Wire_Find_Attempt;

extern int PWM_Right;
extern int PWM_Left;
extern int MAG_Goal;
extern int MAG_Error;
extern int MAG_Start;
extern byte PWM_Blade_Speed_Min;
extern byte PWM_Blade_Speed_Max;
extern bool Blade_Override;

// Compass Variables
extern float Compass_Heading_Degrees;
extern float Heading;
extern bool Compass_Heading_Locked;
extern float Heading_Lock;
extern int Heading_Upper_Limit_Compass;
extern int Heading_Lower_Limit_Compass;
extern float Compass_Target;
extern int Compass_Leg;
extern int Turn_Adjust;
extern int error;
extern float Calb_XAxis;
extern float Calb_YAxis;
extern float Calb_ZAxis;
extern int Tilt_X;
extern int Tilt_Y;
extern int Tilt_Z;
extern float X_Tilt;
extern float Y_Tilt;
extern float Z_Tilt;

// Pattern Mow
extern int Spiral_Mow;
extern int Linking_Section;
extern int Leg;
extern float Compass_Last;

// Wire Track Printer
extern int PrintInMax;
extern int PrintInMid;
extern int PrintInMin;
extern int PrintOutMin;
extern int PrintOutMid;
extern int PrintOutMax;
extern int PrintMAG_Now;

// WIFI Variables
extern float val_WIFI;

// EEPROM Saved Settings
extern int PWM_LEFT_EEPROM;
extern int PWM_RIGHT_EEPROM;
extern int PWM_BLADE_EEPROM;
extern int COMPASS_EEPROM;
extern float Tracking_PID_P_EEPROM;
extern int Pattern_Mow_EEPROM;
extern float Minimum_Volt_EEPROM;
extern int Compass_Home_EEPROM;
extern int Tip_Safety_EEPROM;
extern int Mower_Turn_Delay_Min_EEPROM;
extern int Mower_Turn_Delay_Max_EEPROM;
extern int Mower_Reverse_Delay_EEPROM;
extern bool Sonar_1_Activate_EEPROM;
extern bool Sonar_2_Activate_EEPROM;
extern bool Sonar_3_Activate_EEPROM;
extern int Track_Wire_Zone_1_Cycles_EEPROM;
extern int Track_Wire_Zone_2_Cycles_EEPROM;
extern bool Use_Charging_Station_EEPROM;
extern bool CW_Tracking_To_Charge_EEPROM;
extern bool CCW_Tracking_To_Charge_EEPROM;
extern bool CW_Tracking_To_Start_EEPROM;
extern bool CCW_Tracking_To_Start_EEPROM;
extern int Max_Cycles_Straight_EEPROM;
extern bool Compass_Heading_Hold_Enabled_EEPROM;
extern float CPower_EEPROM;
extern int Max_Sonar_Hit_EEPROM;
extern long maxdistancesonar_EEPROM;
extern bool Perimeter_Wire_Enabled_EEPROM;
extern int Max_Cycle_Wire_Find_EEPROM;
extern int Max_Cycle_Wire_Find_Back_EEPROM;
extern int Max_Tracking_Turn_Right_EEPROM;
extern int Max_Tracking_Turn_Left_EEPROM;
extern bool Rain_Sensor_Installed_EEPROM;
extern int Rain_Total_Hits_Go_Home_EEPROM;
extern bool WIFI_Enabled_EEPROM;
extern bool Cutting_Blades_Activate_EEPROM;
extern int Low_Battery_Instances_Chg_EEPROM;
extern int Alarm_1_Action_EEPROM;
extern bool Bumper_Activate_Frnt_EEPROM;

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
extern bool Cutting_Blades_Activate;

extern bool Perimeter_Wire_Enabled; // Activates use of the perimeter
                                    // boundary wire

// Docking Station
extern bool Use_Charging_Station;   // 1 if you are using the docking/charging
                                    // station, 0 if not
extern bool CW_Tracking_To_Charge;  // Clock-Wise tracking around the
                                    // boundary wire to the charging station
extern bool CCW_Tracking_To_Charge; // Counter-Clock-Wise tracking around the
                                    // boundary wire to the charging station
extern bool CW_Tracking_To_Start;   // Clock-Wise  tracking around the boundary
                                    // wire when tracking to the start position
extern bool CCW_Tracking_To_Start;  // Counter-Clock-Wise tracking around the
                                    // boundary wire to the charging station
extern byte Docked_Filter_Hits;     // Number of charge signals
                                    // to be detected before mower powers off

// Wire Tracking
extern int Track_Wire_Zone_1_Cycles; // Zone 1 - Number of Itterations the
                                     // PID function does before the
                                     // mower exits the wire track
extern int Track_Wire_Zone_2_Cycles; // Zone 2 - Therefore how long the mower is
                                     // tracking the wire can be
                                     // set = distance tracked.
extern int Max_Tracking_Turn_Right;  // The maximum number of turn right
                                     // commands during wire tracking
                                     // before a renewed wire find function
                                     // is called (wheel spins)
extern int Max_Tracking_Turn_Left;   // a re-find the wire sub-routine is
                                     // called if this value is reached.
extern int Max_Cycle_Wire_Find;      // Maximum number of forward tracking
                                     // cycles in finding wire before the
                                     // mower restarts a compass turn
                                     // and wire find.
extern int Max_Cycle_Wire_Find_Back; // Maximum number of Backward tracking
                                     // cycles in finding wire before the
                                     // mower restarts a compass turn and
                                     // wire find.

// Compass Settings
extern bool Compass_Activate;             // Turns on the Compass (needs to be 1 to activate
                                          // further compass features)
extern bool Compass_Heading_Hold_Enabled; // Activates the compass heading hold
                                          // function to keep the mower straight
extern int Home_Wire_Compass_Heading;     // Heading the Mower will search for the
                                          // wire once the mowing is completed.
extern int CPower;                        // Magnification of heading to PWM - How strong the mower
                                          // corrects itself in Compass Mowing

// Pattern Mow
extern int Pattern_Mow;             // EEPROM
                                    // 0 = OFF |  1 = Parallel (not working!!)
                                    // | 3 = Sprials |
extern int Max_Cycles_Spirals;      // Overrides the Max_Cycles for straught line
                                    // mowing as the spirals
                                    // requires more loops to complete
extern float Compass_Mow_Direction; // Mow Direction of line when pattern
                                    // mow is activated

// Safety Tilt Feature
extern int Tip_Safety; // EEPROM // Experimental and not working yet!! - Stops
                       // all motors if the mower overturns or is picked up.
extern int Max_Tilt;
extern int Min_Tilt;
extern int Max_Tilt_Hits;

// Compass alternative settings if the QMC Compass is freezing
extern int Compass_QMC_Refresh_Setting; // 1 = 200HZ and 2 = 100Hz and
                                        // 3 = 50HZ (Standard) 4 = 10Hz if
                                        // compass is freezing try a different
                                        // refresh rate.
extern int Compass_QMC_Sample_Setting;  // Number of samples per call to the
                                        // Compass | 1 = 2 | 2 = 4 |
                                        // 3 = 8 (Standard) |.

// Rain sensor
extern bool Rain_Sensor_Installed;   // 1  = Rain sensor installed
                                     // 0 = no sensor installed.
extern byte Rain_Total_Hits_Go_Home; // This sensor only makes
                                     // sense in combination with a mower
                                     // docking station as the mower is sent
                                     // there to get out of the rain.

// Battery Settings
extern float Battery_Max;              // Max battery volts in Volts. 3S = 12.6V
extern float Battery_Min;              // EEPROM          // Lower Limit of battery charge
                                       // before re-charge required.
extern byte Low_Battery_Detected;      // Always set to 0
extern byte Low_Battery_Instances_Chg; // Instances of low battery detected
                                       // before a re-charge is called..

// Sonar Modules
extern bool Sonar_1_Activate; // Activate (1) Deactivate (0) Sonar 1
extern bool Sonar_2_Activate; // Activate (1) Deactivate (0) Sonar 2
extern bool Sonar_3_Activate; // Activate (1) Deactivate (0) Sonar 3
extern int Max_Sonar_Hit;     // Maximum number of Sonar hits before
                              // object is discovered
extern long maxdistancesonar; // distance in cm from the mower that the sonar
                              // will activate at.

// Bumper Module
extern bool Bumper_Activate_Frnt; // EEPROM // Activates the bumper bar on the front facia - defualt is off.  Enable in the LCD settings menu.

// Wheel Motors Setup
extern int Max_Cycles_Straight; // Number of loops the Sketch will run before
                                // the mower just turns around anyway. Adjust
                                // according to your garden length
extern int PWM_MaxSpeed_LH;     // EEPROM            // Straight line speed LH Wheel
                                // (Looking from back of mower)  Will be overidden
                                // if saved in EEPROM
extern int PWM_MaxSpeed_RH;     // EEPROM            // Straight line speed RH Wheel
                                // - adjust to keep mower tracking straight.  Will
                                // be overridden if saved in EEPROM

extern int Max_Motor_PWM_LH;
extern int Max_Motor_PWM_RH;
extern int Mower_Turn_Delay_Min; // Min Max Turn time of the Mower after it
                                 // reverses at the wire.
extern int Mower_Turn_Delay_Max; // A random turn time between these numbers is
                                 // selected by the software
extern int Mower_Reverse_Delay;  // Time the mower reverses before making a turn.

// Blade Motor Setup
// Blade Speed can be modified in the settings menu and will be written to
// EEPROM The number below will then be overidden
extern byte PWM_Blade_Speed; // EEPROM
                             // PWM signal sent to the blade motor (speed of
                             // blade) new motor works well at 245.

// Alarm Setup
extern bool Set_Time; // Turn to 1 to set time on RTC (Set time in Time tab
                      // Set_Time_On_RTC)
                      // After setting time turn to 0 and reload sketch.

// If the Alarm is changed in settings it will be written to EEPROM and the
// settings below will be overriden. Action for Alarm 1 is set to exit the dock
// and mow at this time. To change this action go to "void Activate_Alarms()"
extern bool Alarm_1_ON;     // EEPROM    // Activate Alarm 1  (1 = ON 0 = OFF)
extern int Alarm_1_Hour;    // EEPROM    // Mowing Hour Number 1
extern int Alarm_1_Minute;  // EEPROM    // Alarm Minute 1
extern bool Alarm_1_Repeat; // Repeat the Alarm at the same time
extern int Alarm_1_Action;  // EEPROM // Sets the actions to be performed when the alarm is called

// Action for Alarm 2 can be set in "void Activate_Alarms()"
extern bool Alarm_2_ON;     // EEPROM // Activate Alarm 2 (1 = ON 0 = OFF)
extern int Alarm_2_Hour;    // EEPROM // Mowing Hour Number 2
extern int Alarm_2_Minute;  // EEPROM // Alarm minute 2
extern bool Alarm_2_Repeat; // Repeat the Alarm at the same time
extern int Alarm_2_Action;  // EEPROM // Sets the actions to be performed when the alarm is called

// Action for Alarm 3 can be set in "void Activate_Alarms()"
// Go Home Alarm
extern bool Alarm_3_ON;     // EEPROM // Activate Alarm 3 (1 = ON 0 = OFF)
extern int Alarm_3_Hour;    // EEPROM // Mowing Hour Number 3
extern int Alarm_3_Minute;  // EEPROM // Alarm minute 3
extern bool Alarm_3_Repeat; // Repeat the Alarm at the same time
extern int Alarm_3_Action;  // EEPROM // Sets the actions to be performed when the alarm is called

extern byte Alarm_Second; // Seconds

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
extern int InMin;
extern int InMid;
extern int InMax; // the maximum received signal value  the wire
/*General Setup PID numbers for wire tracking*/
extern float P;    // EEPROM  // Multiplication factor to the error measured
                   // to the wire center.  if jerky movement when tracking reduce
                   // number
extern float D;    // Dampening value to avoid the mower snaking on the wire.
extern byte Scale; // Serial Monitor Line Tracking Print Scale

// These values set the scale for the wire print out in the serial monitor once
// tracking
extern int OutMin;
extern int OutMid;
extern int OutMax;

extern int Outside_Wire_Count_Max;        // If the mower is outside the wire this many
                                          // times the mower is stopped
extern int Action_On_Over_Wire_Count_Max; // Set 1
                                          // to hibernate
                                          // mower (Power Off
                                          // and Stop)   Set 2 to refind garden
// using sonar and wire detect function
// 3 to do a refind wire function

extern bool Show_TX_Data; // Show the values recieved from the Nano / ModeMCU
                          // in the serial monitor
