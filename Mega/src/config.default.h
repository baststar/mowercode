
/***********************************************************************************************

                   SETUP OF MOWER

  The following setup parameters will setup the mower for your garden
  Turn on or off the settings to defien how you like the mower to behave.

  Settings marked with EEPROM can be adjusted using the mower LCD menu.  Once
changes and saved the settings will override the settings in this menu.  To
clear these settings you need to clear the EEPROM

  1 = Turned ON      0 = Turned OFF       Value = Value set for variable.

****************************************************************************************************/

// Activates the cutting blades and disc
#define CUTTING_BLADES_ACTIVATED 1 // bool

// Activates use of the perimeter boundary wire
#define PERIMETER_WIRE_ENABLED 1 // bool

// Activates the WIFI Fucntions
#define WIFI_ENABLED 1 // bool

// 1 if you are using the docking/charging station, 0 if not
#define USE_CHARGING_STATION 0 // bool

// Show the values recieved from the Nano / ModeMCU in the serial monitor
#define SHOW_TX_DATA 0 // bool

// TIME/ALARM
// Turn to 1 to set time on RTC (Set time in Time tab Set_Time_On_RTC)
// After setting time turn to 0 and reload sketch
#define SET_TIME 0 // bool

// ALARM
// If the Alarm is changed in settings it will be written to EEPROM and the
// settings below will be overriden. Action for Alarm 1 is set to exit the dock
// and mow at this time. To change this action go to "void Activate_Alarms()"
#define ALARM_1_ON 0     // bool EEPROM
#define ALARM_1_HOUR 12  // int  EEPROM
#define ALARM_1_MINUTE 0 // int  EEPROM
#define ALARM_1_REPEAT 0 // bool
#define ALARM_1_ACTION 1 // int EEPROM
#define ALARM_2_ON 0     // bool EEPROM
#define ALARM_2_HOUR 12  // int  EEPROM
#define ALARM_2_MINUTE 0 // int  EEPROM
#define ALARM_2_REPEAT 0 // bool
#define ALARM_2_ACTION 1 // int EEPROM
#define ALARM_3_ON 0     // bool EEPROM
#define ALARM_3_HOUR 12  // int  EEPROM
#define ALARM_3_MINUTE 0 // int  EEPROM
#define ALARM_3_REPEAT 0 // bool
#define ALARM_3_ACTION 1 // int EEPROM
#define ALARM_SECOND 5   // byte

// WIRE DETECTION
/* Description of how the below values are displayed in the Serial Monitor Print
   Out for the wire function (InMax)                   Wire = 0 (OutMax) |
   (InMid)           |           (OutMid)     |
       |--------|--------|------|------|--------|--------|
       |        |        |      |      |        |        |
                      (InMin)       (OutMin)
*/
// Wire detection Values
// IN - OUT: These values are based on the signal received by the wire sensor for your perimeter loop
//    Negative Values for In
// MULTIPLICATION_FACTOR for the error measured to the wire center
//    if jerky movement when tracking reduce number
// DAMPING_VALUE to avoid the mower snaking on the wire
// OUTSIDE_WIRE_COUNT_MAX:
//    If the mower is outside the wire times the mower is stopped
// ACTION_ON_OVER_WIRE_COUNT_MAX:
//    Set to 1 to hibernate mower (Power and Stop)
//    Set to 2 to refind garden using sonar and wire detect function
//    Set to 3 to do a refind wire function
#define IN_MIN -200                                 // int
#define IN_MID -700                                 // int
#define IN_MAX -1500                                // int
#define OUT_MIN 150                                 // int
#define OUT_MID 400                                 // int
#define OUT_MAX 1500                                // int
#define MULTIPLICATION_FACTOR 0.08                  // float EEPROM
#define DAMPING_VALUE 10                            // float
#define SERIAL_MONITOR_LINE_TRACKING_PRINT_SCALE 36 // byte
#define OUTSIDE_WIRE_COUNT_MAX 3                    // int
#define ACTION_ON_OVER_WIRE_COUNT_MAX 3             // int

// RAIN SENSOR
// This sensor only makes sense in combination with a mower docking
// station as the mower is sent there to get out of the rain.
#define RAIN_SENSOR_ENABLED 0      // bool
#define RAIN_TOTAL_HITS_GO_HOME 10 // byte

// TIP SAFETY
// Experimental and not working yet!! - Stops all motors if the mower overturns or is picked up.
#define TIP_SAFETY_ENABLED 0       // bool EEPROM
#define TIP_SAFETY_MAX_TILT 1800   // int
#define TIP_SAFETY_MIN_TILT -1800  // int
#define TIP_SAFETY_MAX_TILT_HITS 5 // int

// MOTOR-SETUP
// MAX_CYCLES_STRAIGHT: Number of loops the Sketch will run before the mower just turns
// PWM_MAX_SPEED_LEFT/RIGHT: Straight line speed LH Wheel (Looking from bac
//    of mower)
// MOWER_TURN_DELAY_MIN/MAX:
//    Min Max Turn time of the Mower after it reverses at the wire.
//    A random turn time between these numbers is selected by the
//    software
// MOWER_REVERSE_DELAY: Time the mower reverses before making a turn.
// PWM_BLADE_SPEED: PWM signal sent to the blade motor (speed of blade)
//    New motor works well at 245
#define MAX_CYCLES_STRAIGHT 150   // int
#define PWM_MAX_SPEED_LEFT 255    // int EEPROM
#define PWM_MAX_SPEED_RIGHT 255   // int EEPROM
#define MAX_MOTOR_PWM_LEFT 255    // int
#define MAX_MOTOR_PWM_RIGHT 255   // int
#define MOWER_TURN_DELAY_MIN 1500 // int
#define MOWER_TURN_DELAY_MAX 2500 // int
#define MOWER_REVERSE_DELAY 100   // int EEPROM
#define PWM_BLADE_SPEED 250       // byte EEPROM

// BATTERY
// LOW_BATTERY_INSTANCES_CHARGE:
//    Instances of low battery detections before a re-charge is called..
#define BATTERY_MAX 12.6                // float EEPROM
#define BATTERY_MIN 11.4                // float EEPROM
#define LOW_BATTERY_INSTANCES_CHARGE 14 // byte

// SONAR
// MAX_SONAR_HIT
//    Maximum number of Sonar hits before object is discovered
// MAX_DISTANCE_SONAR
//    distance in cm from the mower that the sonar will activate at.
#define SONAR_ONE_ENABLED 1   // bool
#define SONAR_TWO_ENABLED 1   // bool
#define SONAR_THREE_ENABLED 1 // bool
#define MAX_SONAR_HIT 3       // int
#define MAX_DISTANCE_SONAR 30 // long

// Bumper Module
// Activates the bumper bar on the front facia - defualt is off.  Enable in the LCD settings menu.
#define BUMPER_ACTIVATE_FRNT 0 // bool

// COMPASS
// Compass alternative settings if the QMC Compass is freezing
// HOME_WIRE_COMPASS_HEADING
//    Heading the Mower will search for the wire once the mowing is completed
// COMPASS_CORRECTION_POWER
//    Magnification of heading to PWM - How strong the mower corrects itself in Compass Mowing
// COMPASS_QMC_REFRESH_SETTING
//    1 = 200HZ and 2 = 100Hz and 3 = 50HZ (Standard) 4 = 10Hz if compass is
//    freezing try a different refresh rate.
// COMPASS_QMC_SAMPLE_SETTING
//    Number of samples per call to the Compass
//    | 1 = 2 | 2 = 4 | 3 = 8 (Standard) |
#define COMPASS_ENABLED 1              // bool
#define COMPASS_HEADING_HOLD_ENABLED 0 // bool
#define HOME_WIRE_COMPASS_HEADING 110  // int
#define COMPASS_CORRECTION_POWER 2     // int
#define COMPASS_QMC_REFRESH_SETTING 3  // int
#define COMPASS_QMC_SAMPLE_SETTING 3   // int

// DOCKING STATION
// CLOCKWISE_TRACKING_TO_CHARGE
//    Clock-Wise tracking around the boundary wire to the charging station
// CLOCKWISE_TRACKING_TO_START
//    Clock-Wise tracking around the boundary wire when tracking to the start position
// DOCKED_FILTER_HITS
//    Number of charge signals to be detected before mower powers off
#define CLOCKWISE_TRACKING_TO_CHARGE 1        // bool
#define COUNTERCLOCKWISE_TRACKING_TO_CHARGE 0 // bool
#define CLOCKWISE_TRACKING_TO_START 0         // bool
#define COUNTERCLOCKWISE_TRACKING_TO_START 1  // bool
#define DOCKED_FILTER_HITS 1                  // byte

// WIRE TRACKING
// TRACK_WIRE_ZONE_1_CYCLES
//    Zone 1 - Number of Itterations the PID function does before the mower exits the wire track
// TRACK_WIRE_ZONE_2_CYCLES
//    Zone 2 - Therefore how long the mower is tracking the wire can be set = distance tracked.
// MAX_TRACKING_TURN_RIGHT
//    The maximum number of turn right commands during wire tracking before a renewed wire find function is called(wheel spins)
// MAX_TRACKING_TURN_LEFT
//    a re-find the wire sub-routine is called if this value is reached.
// MAX_CYCLE_WIRE_FIND
//    Maximum number of forward tracking cycles in finding wire before the mower restarts a compass turn and wire find.
// MAX_CYCLE_WIRE_BACK
//    Maximum number of Backward tracking cycles in finding wire before the mower restarts a compass turn and wire find.
#define TRACK_WIRE_ZONE_1_CYCLES 1300 // int
#define TRACK_WIRE_ZONE_2_CYCLES 2200 // int
#define MAX_TRACKING_TURN_RIGHT 270   // int
#define MAX_TRACKING_TURN_LEFT 270    // int
#define MAX_CYCLE_WIRE_FIND 320       // int
#define MAX_CYCLE_WIRE_BACK 50        // int

// PATTERNS
// PATTERN_MOW
//    0 = OFF |  1 = Parallel (not working!!) | 3 = Sprials |
// MAX_CYCLES_SPIRALS
//    Overrides the Max_Cycles for straught line mowing as the spirals requires more loops to complete
// COMPASS_MOW_DIRECTION
//    Mow Direction of line when pattern mow is activated
#define PATTERN_MOW 0             // int EEPROM
#define MAX_CYCLES_SPIRALS 500    // int
#define COMPASS_MOW_DIRECTION 110 // int

// LCD
#define LCD_ADDR 0x27 // uint8_t

// ARDUINO MEGA PINS
#define PIN_REALTIME_CLOCK_RESET 29 // uint8_t
#define PIN_REALTIME_CLOCK_DATA 30  // uint8_t
#define PIN_REALTIME_CLOCK_SCL 31   // uint8_t

#define PIN_PERIMETER_LEFT A5  // perimeter pin normally A5
#define PIN_PERIMETER_RIGHT A4 // leave open
#define PIN_LED LED_BUILTIN

// Sonar Setup for Front Sonars 1-3
#define PIN_ECHO_1 34 // S1
#define PIN_TRIGGER_1 35
#define PIN_ECHO_2 36 // S2
#define PIN_TRIGGER_2 37
#define PIN_ECHO_3 38 // S3
#define PIN_TRIGGER_3 39

// Membrane Switch
#define PIN_START_KEY 50 // connect wire 1 to pin 2
#define PIN_PLUS_KEY 51  // connect wire 2 to pin 3
#define PIN_MINUS_KEY 52 // connect wire 3 to pin 4
#define PIN_STOP_KEY 53  // connect wire 4 to pin 5

// Pin Setup for the wheel Motor Bridge Controller
// Motor A
#define PIN_ENA 7  // EN Pins need a digital pin with PWM
#define PIN_IN_1 6 // IN Pins dont need digital PWM
#define PIN_IN_2 5
// Motor B
#define PIN_ENB 2  // EN Pins need a digital pin with PWM
#define PIN_IN_3 4 // IN Pins dont need digital PWM
#define PIN_IN_4 3

// Motor Blades
#define PIN_RPWM 8
#define PIN_L_EN 9
#define PIN_R_EN 10

// Relay Switch
#define PIN_RELAY_MOTORS 24

// Bumper
#define Bumper_Switch_Frnt_RH 46 // Define Pin 47 on the MEGA to detect the microswitch
#define Bumper_Switch_Frnt_LH 47 // Define Pin 46 on the MEGA to detect the microswitch

// Membrane Switch
#define Start_Key 50 // connect wire 1 to pin 2
#define Plus_Key 51  // connect wire 2 to pin 3
#define Minus_Key 52 // connect wire 3 to pin 4
#define Stop_Key 53  // connect wire 4 to pin 5
