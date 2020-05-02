
#include <AmpVolts.h>
#include <Apply_Setup.h>
#include <Arduino.h>
#include <BETA_Tip.h>
#include <Compass.h>
#include <EEPROM.h>
#include <EEPROM_Functions.h>
#include <LCD_Display.h>
#include <LiquidCrystal_I2C.h>
#include <MENU_Beta.h>
#include <Manouvers.h>
#include <Menu_Docked.h>
#include <Menu_Motion.h>
#include <Menu_NAVI.h>
#include <Menu_Parked.h>
#include <Menu_Sensors.h>
#include <Menu_Set_Time.h>
#include <Menu_Settings.h>
#include <Menu_Tracking.h>
#include <Motor_Action.h>
#include <MowerFSM.h>
#include <Rain.h>
#include <Sonar.h>
#include <TXRX_NANO.h>
#include <Time_Alarms.h>
#include <WIFI.h>
#include <Wire.h>
#include <Wire_Detection.h>
#include <declarations.h>



void setup() {

    Serial.begin(115200);
    Setup_Run_LCD_Intro();
    Setup_Membrane_Buttons();
    setup_fsm();

    /*
        Serial1.begin(1200); // Open Serial port 1 for the nano communication

        if (WIFI_Enabled == true) {
            Serial2.begin(9600); // If WIFI is on open Serial port 2 for the NodeMCU communication
        }
        Wire.begin(); // start the i2c interface
        Serial.println(" ");
        Serial.println(" ");
        Serial.print(F("ReP_AL Robot :"));
        Serial.println(Version);
        Serial.println(F("==================="));
        Serial.println("");
        Serial.println(F("Starting Mower Setup"));
        Serial.println(F("==================="));
        Load_EEPROM_Saved_Data();
        if (Set_Time == 1) {
            Serial.print(F("Setting Time"));
            Set_Time_On_RTC();
        }
        DisplayTime();
        Serial.println("");
        Prepare_Mower_from_Settings();
        Setup_Run_LCD_Intro();
        Setup_Compass();
        delay(100);
        Setup_Relays();
        Setup_Tip_Safety();
        Setup_Membrane_Buttons();
        Setup_Motor_Pins();
        Setup_ADCMan();
        Setup_Check_Pattern_Mow();
        if (Bumper_Activate_Frnt == true) {
            Setup_Bumper_Bar();
        }

    */
}

void loop() {

    loop_fsm();



    /*
        // Read the Serial Ports for Data
        Read_Serial1_Nano(); // Read the Serial data from the nano
        // Print_Mower_Status(); // Update the Serial monitor with the current mower status.

        // Mower is docked waiting for a command to leave and mow.
        if (Mower_Docked == 1) {
            Print_LCD_Volt_Info(); // Print the voltage to the LCD screen
            Check_if_Charging();
            Print_LCD_Info_Docked(); // Print information to the LCD screen
            Print_Time_On_LCD();
            Check_Membrane_Switch_Input_Docked(); // Check the membrane buttons for any input
            TestforBoundaryWire();                // Test is the boundary wire is live
            // Manouver_Dock_The_Mower();
            // Print_Time_On_Serial();
            // Display_Next_Alarm();
            // Activate_Alarms();
        }

        // Mower is Parked ready to be started / re-started.
        // if (Mower_Parked == 1) {
        //     Print_LCD_Volt_Info(); // Print the voltage to the LCD screen
        //     Check_if_Charging();
        //     Check_if_Raining_From_Nano();         // Checks if the water sensor detects Rain
        //     Print_LCD_Info_Parked();              // Print information to the LCD screen
        //     Check_Membrane_Switch_Input_Parked(); // Check the membrane buttons for any input
        //     TestforBoundaryWire();
        //     Manouver_Park_The_Mower();
        // }

        // Mower is Parked with Low Battery needing manuel charging
        // if (Mower_Parked_Low_Batt == 1) {
        //     Print_LCD_Volt_Info(); // Print the battery voltage
        //     Print_Recharge_LCD();  // Print re-charge on the LCD screen
        //     Check_Membrane_Switch_Input_Parked();
        // }

        // // Lost mower is put into standby mode
        // if (Mower_Error == 1) {
        //     Print_Mower_Error(); // Safety mode incase the mower is lostor in an error state
        //     Check_Membrane_Switch_Input_Parked();
        // }

        // Mower is running cutting the grass.
        // if (Mower_Running == 1) {
        //     Print_LCD_Volt_Info(); // Print the voltage to the LCD screen
        //     Process_Volt_Information(); // Take action based on the voltage readings
        // }

        // if (Mower_Running == 1) {
        //     Check_if_Raining_From_Nano(); // Test the rain sensor for rain. If detected sends the mower home
        // }
        // if (Mower_Running == 1) {
        //     Check_Membrane_Keys_Running(); // Check to see if the mower needs to be stopped via keypad
        // }
        // if (Mower_Running == 1) {
        //     Check_Timed_Mow(); // Check to see if the time to go home has come.
        // }

        if (Mower_Running == 1) {
            TestforBoundaryWire(); // Test is the boundary wire is live
        }

        // if ((Mower_Running == 1) && (Tip_Safety == 1)) {
        //     Compass_Check_Tilt_Angle(); // Tests to see if the mower is overturned.
        // }

        if ((Mower_Running == 1) && (Wire_Detected == 1)) {
            Check_Wire_In_Out(); // Test if the mower is in or out of the wire fence.
        }

        // if ((Mower_Running == 1) && (Wire_Detected == 1) && (Outside_Wire == 0)) {
        //     Check_Sonar_Sensors(); // If the mower is  the boundary wire check the sonars for obsticles and prints to the LCD
        // }

        // lcd.clear();
        // lcd.setCursor(0, 0);
        // lcd.print("1?:" + String(Mower_Running) + " 1?:" + String(Wire_Detected) + " 0?:" + String(Outside_Wire));
        // delay(250);

        // mower running ist 0!!!

        if ((Mower_Running == 1) && (Wire_Detected == 1) && (Outside_Wire == 0) && (Sonar_Hit == 0)) {
            Manouver_Mow_The_Grass2(); // Inputs to the wheel motors / blade motors according to surroundings
        }

        if ((Mower_Running == 1) && (Wire_Detected == 1) && (Outside_Wire == 1) && (Loop_Cycle_Mowing > 0)) {
            Manouver_Turn_Around(); // If outside the wire turn around
        }

        // if ((Mower_Running == 1) && (Wire_Detected == 1) && (Outside_Wire == 0) && (Sonar_Hit == 1)) {
        //     Manouver_Turn_Around_Sonar(); // If sonar hit is detected and mower is  the wire, manouver around obsticle
        // }

        // // WIFI Commands from and to APP
        // if (Manuel_Mode == 1) {
        //     Receive_WIFI_Manuel_Commands();
        // }

        // if (Manuel_Mode == 1) {
        //     Print_LCD_Info_Manuel();
        // }

        // if ((WIFI_Enabled == 1) && (Manuel_Mode == 0)) {
        //     Get_WIFI_Commands(); // TX and RX data from NodeMCU
        // }

        // Serial.println();
    */
}
