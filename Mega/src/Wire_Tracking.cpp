#include "../.pio/libdeps/megaatmega2560/Ardumower/code/tests/perimeterV2/adcman.h"
#include <AmpVolts.h>
#include <Arduino.h>
#include <Compass.h>
#include <Manouvers.h>
#include <Motor_Action.h>
#include <TXRX_NANO.h>
#include <WIFI.h>
#include <Wire_Tracking.h>
#include <config.h>
#include <declarations.h>
#include <math.h>
#include <stdio.h>




//  Prints a visual display of the wire tracking in the Serial Monitor
//  to see how well the wire is being followed.  Adjusting the P value in the settings
//  can improve the wire tracking ability of the mower.

void PrintWirePosition() {

    int PrintMAG_Now = MAG_Now / Scale;
    int PrintInMax = InMax / Scale;
    int PrintInMid = InMid / Scale;
    int PrintInMin = InMin / Scale;
    int PrintOutMin = OutMin / Scale;
    int PrintOutMid = OutMid / Scale;
    int PrintOutMax = OutMax / Scale;

    for (int i = PrintInMax; i <= PrintOutMax; i++) {
        if (i == PrintInMax)
            Serial.print(F("|"));
        if (i == PrintInMid)
            Serial.print(F("|"));
        if (i == PrintInMin)
            Serial.print(F("|"));
        if (i == PrintOutMin)
            Serial.print(F("|"));
        if (i == PrintOutMid)
            Serial.print(F("|"));
        if (i == PrintOutMax)
            Serial.print(F("|"));
        if (i == PrintMAG_Now)
            Serial.print(F("X")); // maybe change to MAG_Lasr...
        if (i == 0)
            Serial.print(F("0"));

        else
            Serial.print(F("."));
    }
    Serial.print(F("|MAG_Now:"));
    Serial.print(MAG_Now);
    Serial.print(F("|"));
}

void From_Wire_Into_Garden() {
    if (CCW_Tracking_To_Charge == 1) {
        SetPins_ToTurnLeft(); // Turn left
        Motor_Action_Turn_Speed();
        delay(MAX_SPIN_TO_WIRE_MILLISECONDS); // Turn left for 0.7seconds
        Motor_Action_Stop_Motors();           // Stop the wheel motors.
        SetPins_ToGoForwards();               // Get ready to move off
        Motor_Action_Turn_Speed();
        delay(300);
        Motor_Action_Stop_Motors(); // Stop the wheel motors.
    } else {
        SetPins_ToTurnRight(); // Turn left
        Motor_Action_Turn_Speed();
        delay(MAX_SPIN_TO_WIRE_MILLISECONDS); // Turn left for 0.7seconds
        Motor_Action_Stop_Motors();           // Stop the wheel motors.
        SetPins_ToGoForwards();               // Get ready to move off
        Motor_Action_Turn_Speed();
        delay(300);
        Motor_Action_Stop_Motors(); // Stop the wheel motors.
    }
}

// Function to follow the wire for a specific amount of time set by the itterations 'I'
// Robot tracks the wire until the itterations are exhausted.
void Track_Wire_From_Dock_to_Zone_X(int zone) {

    int trackingTime = 0;
    if (zone == 1) {
        trackingTime = TIME_FROM_DOCK_TO_ZONE_1_MILLISECONDS;
    } else if (zone == 2) {
        trackingTime = TIME_FROM_DOCK_TO_ZONE_2_MILLISECONDS;
    }

    // Exit Dock
    Manouver_Exit_From_Docking_Station();
    Read_Serial1_Nano();

    // Find Wire
    Manouver_Find_Wire_Track2();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DREHEN z: " + String(zone));
    delay(1000);

    Manouver_SpinMowerToWire();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("zum kabel gedreht");
    lcd.setCursor(0, 1);
    lcd.print("jetzt am kabel entlang");
    delay(1000);

    // Track Wire for X Milliseconds
    Track_Perimeter_Wire(trackingTime);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("stop");
    lcd.setCursor(0, 1);
    lcd.print("jetzt in den Garten");
    delay(1000);

    // Move from Wire into garden
    From_Wire_Into_Garden();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tracking done:starting now");


    Tracking_Wire = 0;
    Loop_Cycle_Mowing = 0;

    // delay(100);
    // ADCMan.run();
    // if (Blade_Override == 1) {
    //     Motor_Action_Spin_Blades();
    // }
    // lcd.setCursor(0, 0);
    // lcd.print("Exit Docking to"); // into the garden at a good position to start Mowing
    // lcd.setCursor(2, 1);
    // if (Exit_Zone == 1)
    //     lcd.print("Zone 1");
    // if (Exit_Zone == 2)
    //     lcd.print("Zone 2");
    // delay(1000); // Prints info to LCD display

    // Tracking_Turn_Right = 0; // resets the tracking errors for LH and RH.
    // Tracking_Turn_Left = 0;
    // Tracking_Turn_Right_Milliseconds = 0;
    // Tracking_Turn_Left_Milliseconds = 0;

    // // uses the PID settings in the setup
    // Serial.print(F("P = "));
    // Serial.print(P);
    // Serial.print(F(" / D = "));
    // Serial.print(D);
    // Serial.println(F(""));
    // Tracking_Wire = 1;
    // Mower_Running = 0;
    // MAG_Now = 0; // Reset Values
    // MAG_Start = 0;
    // MAG_Error = 0;
    // MAG_Goal = 0;
    // int Dock_Cycles = 0;

    // delay(500);

    // if (WIFI_Enabled == 1) {
    //     Get_WIFI_Commands();
    // }
    // delay(5);
    // lcd.setCursor(10, 1);
    // lcd.print("0");

    // for (I = 0; I < Track_Wire_Itterations; I++) { // Iterations 'I' before mower leaves the wire.

    //     if (Mower_Parked == 0) {

    //         delay(5);
    //         ADCMan.run();
    //         MAG_Start = perimeter.getMagnitude(0); // Gets the signal strength of the sensor
    //         MAG_Now = MAG_Start;
    //         delay(5);
    //         MAG_Error = (MAG_Goal - MAG_Start); // Calculates the Error to the center of the wire which is normally zero magnitude
    //         PrintWirePosition();                // Prints the overview to the Serial Monitor.
    //         // Check_Wire_Blockage();                                              // homework idea is to avoid anything in the way

    //         // Tracks the wire from the docking station in a Counter-Clockwise direction to the start position

    //         if (CCW_Tracking_To_Start == 1) {
    //             if (MAG_Error > 0) { // If the MAG_Error > 0 then turn right for CCW Tracking. PWM_left is set to max to turn right.
    //                 // TURN RIGHT
    //                 PWM_Left = PWM_MaxSpeed_LH;        // sets the PWM to the max possible for the wheel
    //                 PWM_Right = 255 - (MAG_Error * P); // Mag_Error * P is the value reduced from the max value set PWM and sent to the PWM
    //                 if (PWM_Right > 255)
    //                     PWM_Right = 255; // PWM_Right capped to Max PWM of 255.
    //                 if (PWM_Right >= 0) {
    //                     SetPins_ToGoForwards();
    //                     lcd.setCursor(15, 1);
    //                     lcd.print(" ");
    //                 }

    //                 if (PWM_Right < 0) { // sets the mower to sharp turn to the right (wheels spin opposite) if the Error to the wire is large enough.
    //                     PWM_Right = (-1 * PWM_Right) + 220;
    //                     if (PWM_Right > 255)
    //                         PWM_Right = 255;
    //                     if (PWM_Right >= 0)
    //                         SetPins_ToTurnRight();
    //                     delay(5);
    //                     lcd.setCursor(15, 1);
    //                     lcd.print(">");
    //                 }

    //                 Motor_Action_Dynamic_PWM_Steering(); // Carries out the wheel PWM changes for steering on the wire
    //                 Serial.print(F(" Turn Right "));
    //                 Tracking_Turn_Left = 0; // Failsafe if the mower looses the wire.  If the mower is commanded to turn left or right
    //                 Tracking_Turn_Right_Milliseconds = 0;
    //                 Tracking_Turn_Right = Tracking_Turn_Right + 1;       // too many times it is assumed that the mower is spinning and cant get back on the wire.
    //                 if (Tracking_Turn_Right > Max_Tracking_Turn_Right) { // if this is detected then a function is ran to find the wire again.
    //                     Motor_Action_Stop_Motors();
    //                     lcd.clear();
    //                     lcd.print("Right Wheel");
    //                     lcd.print("Tracking_Turn_Right");
    //                     delay(2000);
    //                     lcd.clear();
    //                     Tracking_Restart_Blocked_Path();
    //                 }
    //             }
    //             if (MAG_Error <= 0) { // If the MAG_Error < 0 then turn left for CCW Tracking
    //                 // TURN LEFT
    //                 PWM_Right = 255;                  // PWM_Right set to max to rotate the mower to the left.
    //                 PWM_Left = 255 + (MAG_Error * P); // + as mag_error is negative to adjust PWM
    //                 if (PWM_Left > 255)
    //                     PWM_Left = 255; // PWM_Left capped to mex PWM of 255
    //                 if (PWM_Left >= 0) {
    //                     SetPins_ToGoForwards();
    //                     lcd.setCursor(0, 1);
    //                     lcd.print(" ");
    //                 }

    //                 if (PWM_Left < 0) { // sets the mower to sharp turn to the left (wheels spin opposite) if the Error to the wire is large enough.
    //                     PWM_Left = (-1 * PWM_Left) + 220;
    //                     if (PWM_Left > 255)
    //                         PWM_Left = 255;
    //                     SetPins_ToTurnLeft();
    //                     delay(5);
    //                     lcd.setCursor(0, 1);
    //                     lcd.print("<");
    //                 }

    //                 Motor_Action_Dynamic_PWM_Steering();
    //                 Serial.print(F(" Turn Left "));
    //                 Tracking_Turn_Right = 0;
    //                 Tracking_Turn_Right_Milliseconds = 0;
    //                 Tracking_Turn_Left = Tracking_Turn_Left + 1;
    //                 if (Tracking_Turn_Left > Max_Tracking_Turn_Left) {
    //                     Motor_Action_Stop_Motors();
    //                     lcd.clear();
    //                     lcd.print("Left Wheel");
    //                     lcd.print("Tracking_Turn_Left");
    //                     delay(2000);
    //                     lcd.clear();
    //                     Tracking_Restart_Blocked_Path();
    //                 }
    //             }
    //             Serial.print(F(" : MAG_Error="));
    //             Serial.println(MAG_Error);
    //             Dock_Cycles = Dock_Cycles + 1;
    //             Loop_Cycle_Mowing = I;
    //             if (Dock_Cycles > 10) {
    //                 Tracking_Wire = Tracking_Wire + 1; // Makes the wire tracking LED in the app blink.
    //                 if (Tracking_Wire > 1)
    //                     Tracking_Wire = 0;
    //                 if (WIFI_Enabled == 1)
    //                     Get_WIFI_Commands();
    //                 Dock_Cycles = 0;
    //             }
    //         }
    //     }

    //     if (CW_Tracking_To_Start == 1) {
    //         // Add Code here for CW tracking to the exit zone.
    //         // Use the code above for CCW tracking to the docking station
    //         // as a template.
    //     }
    //     lcd.setCursor(10, 1);
    //     lcd.print(I);
    // }
    // lcd.clear();
    // Tracking_Wire = 0;
    // Loop_Cycle_Mowing = 0;

    // delay(5);
}


//  Track the Perimeter wire using a PID type method
//  This code tracks the boundary wire and by calculating the Perimeter Magnitude and calculating the distance to the
//  center of the wire. This error value of (position - center of wire) is mulitplied by the P value in the setup to
//  send a PWM change to the left or right wheel to bring the sensor back over the wire.*/


int sFunction(double magnitude, double maxMagnitude, int maxSpeed) {

    double cappedMagnitude = magnitude;

    if (cappedMagnitude > maxMagnitude) {
        cappedMagnitude = maxMagnitude;
    }

    if (cappedMagnitude > 0) {
        cappedMagnitude = cappedMagnitude * 1.8;
    } else if (cappedMagnitude < 100) {
        cappedMagnitude = cappedMagnitude + 100;
    }

    float percent = (abs(cappedMagnitude) / maxMagnitude) * 100;

    // lcd.clear();
    // lcd.setCursor(0, 0);
    // lcd.print("p " + String(percent) + " r " + int(percent * maxSpeed / 100));
    // lcd.setCursor(0, 1);
    // lcd.print("" + String(percent) + " | " + String(percent));

    return int(percent * maxSpeed / 100);
}

// if time = 0, mower tracks to the dock
void Track_Perimeter_Wire(int maxTrackingTimeMilliseconds = 0) {

    Tracking_Wire = 1;
    Loop_Cycle_Mowing = 0;
    Mower_Running = 0;
    Docked_Hits = 0;
    Motor_Action_Stop_Spin_Blades();
    Read_Serial1_Nano();
    Check_if_Charging(); // Checks if an amperage is detected on the charge wire
    Check_if_Docked();
    Tracking_Turn_Left_Milliseconds = 0;
    Tracking_Turn_Right_Milliseconds = 0;
    // int Dock_Cycles = 0;

    SetPins_ToGoForwards();
    ADCMan.run();
    inside = perimeter.isInside(0);
    bool lastInside = inside;
    long loopTime = 0;

    unsigned long lastMowerCheckWireMillis = 0;

    while ((Mower_Docked == 0) && (Mower_Parked == 0)) {

        unsigned long currentMillis = millis();

        // only every TRACKING_WIRE_REFRESH_INTERVAL
        if ((currentMillis - lastMowerCheckWireMillis) >= TRACKING_WIRE_REFRESH_INTERVAL) {

            unsigned long millisDiff = 0;

            if (lastMowerCheckWireMillis > 0) {
                millisDiff = currentMillis - lastMowerCheckWireMillis;
            }

            if (maxTrackingTimeMilliseconds > 0) {
                loopTime = loopTime + millisDiff;
                if (loopTime >= maxTrackingTimeMilliseconds) {
                    break;
                }
            }

            lastMowerCheckWireMillis = currentMillis;

            ADCMan.run();
            int asideValue = perimeter.getMagnitude(0);
            inside = perimeter.isInside(0);

            if (asideValue > 0) { // Trun the mower to the right if MAG_Error > 0 with a CCW track direction.

                if (lastInside != inside) {
                    Tracking_Turn_Left_Milliseconds = 0;
                    Tracking_Turn_Right_Milliseconds = 0;
                    lastInside = inside;
                }

                if (CCW_Tracking_To_Charge == 1) {

                    // LEFT TURN
                    PWM_Right = SLOWEST_POSSIBLE_SPEED_VALUE; // Set the PWM_Right to maximum

                    PWM_Left = SLOWEST_POSSIBLE_SPEED_VALUE - sFunction(asideValue, MAX_TRACKING_WIRE_MAGNITUDE, SLOWEST_POSSIBLE_SPEED_VALUE);

                    // PWM_Left = SLOWEST_POSSIBLE_SPEED_VALUE - (abs(asideValue) * P); // + as mag_error is negative to adjust PWM

                    if (PWM_Left < 0) {
                        PWM_Left = 0;
                        // SetPins_ToTurnLeft();
                    }


                    if (PWM_Left > SLOWEST_POSSIBLE_SPEED_VALUE) {
                        PWM_Left = SLOWEST_POSSIBLE_SPEED_VALUE; // cap PWM_Left to the maximum
                    }

                    Tracking_Turn_Right = 0; // Failsafe if the mower looses the wire.  If the mower is commanded to turn left or right
                    Tracking_Turn_Right_Milliseconds = 0;
                    Tracking_Turn_Left = Tracking_Turn_Left + 1; // too many times it is assumed that the mower is spinning and cant get back on the wire.
                    Tracking_Turn_Left_Milliseconds = Tracking_Turn_Left_Milliseconds + millisDiff;

                    if (Tracking_Turn_Left_Milliseconds > Max_Tracking_Turn_Left_Milliseconds) { // if this is detected then a function is ran to find the wire again.
                        // lcd.clear();
                        // lcd.setCursor(0, 0);
                        // lcd.print("error 0 Restart_Blocked_Path2");
                        // delay(1000);
                        // Tracking_Restart_Blocked_Path2();
                        SetPins_ToTurnLeft();
                        PWM_Left = SLOWEST_POSSIBLE_SPEED_VALUE;
                        PWM_Right = SLOWEST_POSSIBLE_SPEED_VALUE;
                    } else {
                        SetPins_ToGoForwards();
                    }

                } else {
                }
            } else if (asideValue < 0) {

                if (lastInside != inside) {
                    Tracking_Turn_Left_Milliseconds = 0;
                    Tracking_Turn_Right_Milliseconds = 0;
                    lastInside = inside;
                }

                if (CCW_Tracking_To_Charge == 1) {

                    // RIGHT TURN
                    PWM_Left = SLOWEST_POSSIBLE_SPEED_VALUE; // Set PWM_Left to maximum
                    PWM_Right = SLOWEST_POSSIBLE_SPEED_VALUE - sFunction(asideValue, MAX_TRACKING_WIRE_MAGNITUDE, SLOWEST_POSSIBLE_SPEED_VALUE);

                    if (PWM_Right < 0) {
                        PWM_Right = 0;
                    }

                    if (PWM_Right > SLOWEST_POSSIBLE_SPEED_VALUE) {
                        PWM_Right = SLOWEST_POSSIBLE_SPEED_VALUE; // Caps the PWM_Right to 255
                    }

                    Tracking_Turn_Left = 0; // Failsafe if the mower looses the wire.  If the mower is commanded to turn left or right
                    Tracking_Turn_Left_Milliseconds = 0;
                    Tracking_Turn_Right = Tracking_Turn_Right + 1;                                    // too many times it is assumed that the mower is spinning and cant get back on the wire.
                    Tracking_Turn_Right_Milliseconds = Tracking_Turn_Right_Milliseconds + millisDiff; // too many times it is assumed that the mower is spinning and cant get back on the wire.

                    if (Tracking_Turn_Right_Milliseconds > Max_Tracking_Turn_Right_Milliseconds) { // if this is detected then a function is ran to find the wire again.
                        // lcd.clear();
                        // lcd.setCursor(0, 0);
                        // lcd.print("error 1 Restart_Blocked_Path2");
                        // delay(1000);
                        SetPins_ToTurnRight();
                        PWM_Left = SLOWEST_POSSIBLE_SPEED_VALUE;
                        PWM_Right = SLOWEST_POSSIBLE_SPEED_VALUE;
                    } else {
                        SetPins_ToGoForwards();
                    }

                } else {
                }
            }

            if (PWM_Left > SLOWEST_POSSIBLE_SPEED_VALUE) {
                PWM_Left = SLOWEST_POSSIBLE_SPEED_VALUE;
            }

            if (PWM_Right > SLOWEST_POSSIBLE_SPEED_VALUE) {
                PWM_Right = SLOWEST_POSSIBLE_SPEED_VALUE;
            }

            Motor_Action_Dynamic_PWM_Steering();

            Read_Serial1_Nano();
            Check_if_Charging();
            Check_if_Docked();

        } // if ((currentMillis - lastMowerCheckWire) >= TRACKING_WIRE_REFRESH_INTERVAL) {
    }

    Motor_Action_Stop_Motors();
    SetPins_ToGoForwards();
    Read_Serial1_Nano();
    Check_if_Charging();
    Check_if_Docked();

    Loop_Cycle_Mowing = 0;
    Tracking_Wire = 0;
}


// Starts an algorithym to find the wire again after it is lost in tracking
void Tracking_Restart_Blocked_Path2() {

    Motor_Action_Stop_Motors();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("start Tracking_Restart_Blocked_Path2");
    delay(1000);


    Serial.println(F("Possible Blocked Path - Trying to Avoid"));
    Mower_Running = 1;
    Tracking_Wire = 1;
    if (WIFI_Enabled == 1) {
        Get_WIFI_Commands(); // TX and RX data from NodeMCU
    }

    delay(1000);
    Mower_Running = 0;
    Tracking_Wire = 0;
    if (WIFI_Enabled == 1) {
        Get_WIFI_Commands(); // TX and RX data from NodeMCU
    }

    delay(1000);
    Mower_Running = 1;
    Tracking_Wire = 1;
    if (WIFI_Enabled == 1) {
        Get_WIFI_Commands(); // TX and RX data from NodeMCU
    }

    delay(1000);
    Mower_Running = 0;
    Tracking_Wire = 0;
    if (WIFI_Enabled == 1) {
        Get_WIFI_Commands(); // TX and RX data from NodeMCU
    }

    lcd.clear();
    lcd.print("Wire Lost.");
    lcd.setCursor(0, 1);
    lcd.print("Recovering....."); // Prints info to LCD display

    delay(1000);


    if (WIFI_Enabled == 1) {
        Get_WIFI_Commands(); // TX and RX data from NodeMCU
    }

    if (Mower_Parked != 1) { // If Pause has been pressed dont carry on.

        SetPins_ToGoBackwards();
        Motor_Action_Turn_Speed();

        delay(2000); // Reversing Time in seconds

        Motor_Action_Stop_Motors();
        Tracking_Turn_Left = 0; // Resets the tracking error counters
        Tracking_Turn_Left_Milliseconds = 0;
        Tracking_Turn_Right = 0; // Resets the tracking error counters
        Tracking_Turn_Right_Milliseconds = 0;

        delay(500);

        Mower_Running = 0;
        Tracking_Wire = 0;

        if (WIFI_Enabled == 1) {
            Get_WIFI_Commands(); // TX and RX data from NodeMCU
        }

        if (Compass_Activate == true) {
            // Compass_Turn_Mower_To_Home_Direction();
        }

        Manouver_Find_Wire_Track2();
        Manouver_SpinMowerToWire();
        // Track_Perimeter_Wire_To_Dock2();
        Track_Perimeter_Wire(0);
    }
}
