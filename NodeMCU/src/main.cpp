
/* Comment this out to disable prints and save space */

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.

#include <Arduino.h>

#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <config.h>

#include <main.h>

#define BLYNK_PRINT Serial

//#define LED LED_BUILTIN
#define LED D4 // on various NODEMCU boards the LED Pin is different.....

SoftwareSerial NodeMCU(D2, D3);
BlynkTimer timer;
WidgetLCD lcd(V6);

int buttonState = 0;

bool Mower_Already_Started = false;
bool Going_Home_Already = false;

int transmit_blynk_code = 0;

int loopstatus = 0;
int lastConnectionAttempt = millis();
int connectionDelay = 5000; // try to reconnect every 5 seconds

// RX Variables
float val_VoltNow;     // Battery Voltage from MEGA
float BatteryVoltage;  // Voltage read last RX
int Loop_Cycle_Mowing; // Current Loop Cycle from MEGA

int Mower_Parked;
int Mower_Parked_Low_Batt;
int Mower_Docked;
int Mower_Docked_Filter;
int Mower_Running;
int Mower_Running_Filter;
int Mower_Lost;

int Charge_Detected;
int Tracking_Wire;
int Compass_Heading_Lock;

int i = 0; // Transmit blank code
int j = 0;
int k = 0;

bool Manuel_Mode;
int Automatic_Mode;

int All;
int RX_Data_Recieved;

void setup()
{
    Serial.begin(9600);
    NodeMCU.begin(9600);
    Serial.println();
    Serial.println("Setting up NODEMCU........");
    Serial.println("");
    pinMode(D2, INPUT);
    pinMode(D3, OUTPUT);
    digitalWrite(LED, HIGH); // Turn off LED Light
    WIFI_Connect();          // Connect to the WIFI
    Clear_APP();
    lcd.clear();
    pinMode(LED, OUTPUT);
}

void loop()
{
    loopstatus = loopstatus + 1;
    RX_Data_Recieved = 0; // resets the data received bool

    if (!Blynk.connected()) {
        Blynk.run();
        timer.run();
        digitalWrite(LED, HIGH);
        Serial.println("------------------------");
        Serial.println("NODEMCU Disconnected");
        Serial.println("Reconnecting ... ");
        WIFI_Connect();
    }
    else {
        Blynk.run();
        timer.run();
        digitalWrite(LED, LOW); // LED is inverted on a MODEMCU...
        Receive_All_From_MEGA();
        Update_Blynk_App_With_Status();
        Serial.println(""); // new line serial monitor
    }
}

void myTimerEvent()
{
    if ((BatteryVoltage > 10) && (BatteryVoltage < 15)) {
        Blynk.virtualWrite(V3, BatteryVoltage);
    }

    Blynk.virtualWrite(V5, Loop_Cycle_Mowing);
}

// Functions
//***********************************

void WIFI_Connect()
{
    unsigned mytimeout = millis() / 1000;
    Serial.println();
    Serial.println("***************** WIFI_CONNECT *****************");
    Blynk.begin(BLYNK_APP_TOKEN, WLAN_SSID, WLAN_PASSWORD, IPAddress(139, 59, 206, 133), 8080);

    while (Blynk.connected() != WL_CONNECTED) {
        delay(500);
        if ((millis() / 1000) > mytimeout + 3) { // try for less than 4 seconds to connect to WiFi router
            break;
        }
    }

    if (!Blynk.connected()) {
        digitalWrite(LED, HIGH);
        Serial.println("NODEMCU Disconnected");
        Serial.println("Reconnecting . . . . . . ");
        WIFI_Connect();
    }
    else {
        digitalWrite(LED, LOW);
        Serial.println("Connected. . . . . .");
        // Setup a function to be called every second
        timer.setInterval(1000L, myTimerEvent);
    }
}

void Update_Blynk_App_With_Status()
{
    if (Mower_Docked_Filter == 2) {
        // App Buttons
        Blynk.virtualWrite(V10, LOW); // Exit Dock Button
        Blynk.virtualWrite(V0, LOW);  // Quick Start Button
        Blynk.virtualWrite(V2, LOW);  // Pause/Stop Button
        Blynk.virtualWrite(V1, LOW);  // Go To Dock Button
    }

    if (Mower_Running_Filter == 2) {
        // App Buttons
        Blynk.virtualWrite(V10, LOW); // Exit Dock Button
        Blynk.virtualWrite(V0, LOW);  // Quick Start Button
        Blynk.virtualWrite(V2, LOW);  // Pause/Stop Button
        Blynk.virtualWrite(V1, LOW);  // Go To Dock Button
    }

    if (Manuel_Mode == true) {
        // App Buttons
        Blynk.virtualWrite(V10, LOW); // Exit Dock Button
        Blynk.virtualWrite(V0, LOW);  // Quick Start Button
        Blynk.virtualWrite(V2, LOW);  // Pause/Stop Button
        Blynk.virtualWrite(V1, LOW);  // Go To Dock Button

        Blynk.virtualWrite(V6, 0);
        Blynk.virtualWrite(V7, 0);
        Blynk.virtualWrite(V9, 0);
        Blynk.virtualWrite(V8, 0);
    }

    if (Mower_Parked == 1) {
        // App Buttons
        Blynk.virtualWrite(V10, LOW); // Exit Dock Button
        Blynk.virtualWrite(V0, LOW);  // Quick Start Button
        Blynk.virtualWrite(V2, LOW);  // Pause/Stop Button
        Blynk.virtualWrite(V1, LOW);  // Go To Dock Button
    }

    if (Charge_Detected == 4)
        Blynk.virtualWrite(V12, 1023); // Charging LED ON
    if (Charge_Detected == 0)
        Blynk.virtualWrite(V12, 0); // Charging LED OFF
    if (Tracking_Wire == 1)
        Blynk.virtualWrite(V9, 1023); // Tracking LED ON
    if (Tracking_Wire == 0)
        Blynk.virtualWrite(V9, 0); // Tracking LED OFF
    if (Mower_Parked == 1)
        Blynk.virtualWrite(V6, 1023); // Parked LED
    if (Mower_Parked == 0)
        Blynk.virtualWrite(V6, 0); // Parked LED
    if (Mower_Running_Filter == 2)
        Blynk.virtualWrite(V7, 1023); // Mowing LED
    if (Mower_Running_Filter != 2)
        Blynk.virtualWrite(V7, 0); // Mowing LED
    if (Mower_Docked_Filter == 2)
        Blynk.virtualWrite(V8, 1023); // Docked LED
    if (Mower_Docked_Filter != 2)
        Blynk.virtualWrite(V8, 0); // Docked LED
}

BLYNK_WRITE(V0) // Quick Start Mowing Function
{
    Clear_APP();

    int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    if (pinValue == 1)
        StartMower();
    // Clear_APP();
    lcd.clear();
    lcd.print(0, 0, "Starting . . "); // Print to APP LCD
    delay(100);
    Blynk.virtualWrite(V0, HIGH); // Start Mowing Buton ON
}

BLYNK_WRITE(V1) // Go to Docking Station
{
    int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    if (pinValue == 1)
        Going_Home();
    // Clear_APP();
    lcd.clear();
    lcd.print(0, 0, "Docking . . "); // Print to APP LCD
    delay(100);
    Blynk.virtualWrite(V1, HIGH); // Docking Button ON
}

BLYNK_WRITE(V2) // Pause Mower Function
{
    int pinValue = param.asInt(); // assigning incoming value from pin V2 to a variable
    if (pinValue == 1)
        Pause_Mower();
    // Clear_APP();
    lcd.clear();
    lcd.print(0, 0, "Stopping . . "); // Print to APP LCD
    delay(100);
    Blynk.virtualWrite(V2, HIGH); // Pause Button ON
}

BLYNK_WRITE(V10) // Exit Dock Function
{
    int pinValue = param.asInt(); // assigning incoming value from pin V10 to a variable
    if (pinValue == 1)
        Exit_Dock();
    // Clear_APP();
    lcd.clear();
    lcd.print(0, 0, "Exiting . . "); // Print to APP LCD
    delay(100);
    Blynk.virtualWrite(V10, HIGH); // Dock Button ON
}

BLYNK_WRITE(V13) // Manuel Forward Motion
{
    if (Manuel_Mode == true) {
        int pinValue = param.asInt(); // assigning incoming value from pin V10 to a variable
        if (pinValue == 1)
            transmit_blynk_code = 7;
        Transmit_Blynk_Data_to_Mega();
        Blynk.virtualWrite(V13, LOW); // Start Mowing Buton ON
    }
}

BLYNK_WRITE(V14) // Manuel Reverse Motion
{
    if (Manuel_Mode == true) {
        int pinValue = param.asInt(); // assigning incoming value from pin V10 to a variable
        if (pinValue == 1)
            transmit_blynk_code = 8;
        Transmit_Blynk_Data_to_Mega();
    }
}

BLYNK_WRITE(V15) // Manuel Left Turn
{
    if (Manuel_Mode == true) {
        int pinValue = param.asInt(); // assigning incoming value from pin V10 to a variable
        if (pinValue == true)
            transmit_blynk_code = 9;
        Transmit_Blynk_Data_to_Mega();
    }
}

BLYNK_WRITE(V16) // Manuel Right Turn
{
    if (Manuel_Mode == true) {
        int pinValue = param.asInt(); // assigning incoming value from pin V10 to a variable
        if (pinValue == 1)
            transmit_blynk_code = 10;
        Transmit_Blynk_Data_to_Mega();
    }
}

BLYNK_WRITE(V4)
{
    switch (param.asInt()) {
    case 1: { // Item 1
        Serial.println("Automatic Mode");
        Automatic_Mode = 1;
        Manuel_Mode = 0;
        Set_To_Automatic_Mode_Random();
        Update_Blynk_App_With_Status();
        break;
    }
    case 2: { // Item 2
        Serial.println("Automatic Spiral");
        Automatic_Mode = 2;
        Manuel_Mode = 0;
        Set_To_Automatic_Mode_Spiral();
        Update_Blynk_App_With_Status();
        break;
    }

    case 3: { // Item 3
        Serial.println("Automatic Parallel");
        Manuel_Mode = 0;
        Automatic_Mode = 1;
        Set_To_Automatic_Mode_Parallel();
        Mower_Parked = 0;
        Mower_Docked = 0;
        Mower_Running = 0;
        Update_Blynk_App_With_Status();
        break;
    }

    case 4: { // Item 4
        Serial.println("Manuel Mode");
        Manuel_Mode = 1;
        Automatic_Mode = 0;
        Set_To_Manuel_Mode();
        Mower_Parked = 0;
        Mower_Docked = 0;
        Mower_Running = 0;
        Update_Blynk_App_With_Status();
        break;
    }
    }
}

void Clear_APP()
{
    Blynk.virtualWrite(V0, LOW);  // Start Mowing Buton OFF
    Blynk.virtualWrite(V1, LOW);  // Go-Home Button OFF
    Blynk.virtualWrite(V2, LOW);  // Stop Button OFF
    Blynk.virtualWrite(V3, 0);    // Reset Voltage
    Blynk.virtualWrite(V5, 0);    // Loops
    Blynk.virtualWrite(V7, 0);    // Mow LED
    Blynk.virtualWrite(V8, 0);    // Dock LED Off
    Blynk.virtualWrite(V9, 0);    // TrackingLED OFF
    Blynk.virtualWrite(V10, LOW); // Dock Button OFF
    Blynk.virtualWrite(V11, 0);   // Compass LED OFF
    Blynk.virtualWrite(V12, 0);   // Charging LED OFF
}

void Transmit_Blynk_Data_to_Mega()
{
    i = transmit_blynk_code + 10; // Transmits the code and adds 10 to the number
    NodeMCU.print(i);
    NodeMCU.println("\p");
    delay(30);
    Serial.print("TX_Code_to_MEGA:");
    Serial.print(i);
    Serial.print("");
    delay(20);
}

void Receive_All_From_MEGA()
{
    String NodeMCU_RX_Value = ""; // changed to string

    while (NodeMCU.available() > 0) {
        char recieved = NodeMCU.read();
        if (recieved != '\g' && recieved != '\c' && recieved != '\d' && recieved != '\z' && recieved != '\y' &&
            recieved != '\o' && recieved != '\m') {
            NodeMCU_RX_Value = NodeMCU_RX_Value + (char)recieved; // hack to joining chars in correct way to Serial1_Rx_Value
        }

        else if (recieved == '\g') {
            val_VoltNow = NodeMCU_RX_Value.toFloat(); // if end of value found, set AmpsTX and clear Serial1_Rx_Value temp var Serial1_Rx_Value used for holding value until \q or \j
            NodeMCU_RX_Value = "";                    // changed to string
        }
        else if (recieved == '\c') {
            Loop_Cycle_Mowing = NodeMCU_RX_Value.toInt(); // same as upper but for VoltsTX,
            NodeMCU_RX_Value = "";
        }
        else if (recieved == '\d') {
            Mower_Docked = NodeMCU_RX_Value.toInt(); // same as upper but for VoltsTX
            NodeMCU_RX_Value = "";
        }
        else if (recieved == '\z') {
            Mower_Running = NodeMCU_RX_Value.toInt(); // same as upper but for VoltsTX
            NodeMCU_RX_Value = "";
        }
        else if (recieved == '\y') {
            Mower_Parked = NodeMCU_RX_Value.toInt(); // same as upper but for VoltsTX
            NodeMCU_RX_Value = "";
        }
        else if (recieved == '\o') {
            Charge_Detected = NodeMCU_RX_Value.toInt(); // same as upper but for VoltsTX
            NodeMCU_RX_Value = "";
        }
        else if (recieved == '\m') {
            Tracking_Wire = NodeMCU_RX_Value.toInt(); // same as upper but for VoltsTX
            NodeMCU_RX_Value = "";
        }
        else
            Serial.print("No Data Received|");
    }
    Print_RX_Values();
    Calculate_Filtered_Mower_Status();
}

void Print_RX_Values()
{
    BatteryVoltage = val_VoltNow;
    Serial.print("Volt:");
    Serial.print(BatteryVoltage);
    Serial.print("|");

    Serial.print("Loop:");
    Serial.print(Loop_Cycle_Mowing);
    Serial.print("|");

    Serial.print("Docked:");
    Serial.print(Mower_Docked);
    Serial.print("|");

    Serial.print("Running:");
    Serial.print(Mower_Running);
    Serial.print("|");

    Serial.print("Parked:");
    Serial.print(Mower_Parked);
    Serial.print("|");

    Serial.print("Charge:");
    Serial.print(Charge_Detected);
    Serial.print("|");

    Serial.print("Tracking:");
    Serial.print(Tracking_Wire);
    Serial.print("|");
}

void Calculate_Filtered_Mower_Status()
{
    if (Mower_Docked == 1) {
        Mower_Docked_Filter = Mower_Docked_Filter + 1;
        if (Mower_Docked_Filter == 3)
            Mower_Docked_Filter = 2;
    }
    else
        Mower_Docked_Filter = 0;

    if (Mower_Running == 1) {
        Mower_Running_Filter = Mower_Running_Filter + 1;
        if (Mower_Running_Filter == 3)
            Mower_Running_Filter = 2;
    }
    else
        Mower_Running_Filter = 0;
}

void Pause_Mower()
{
    Serial.println("");
    Serial.println("Pause/Stop");
    transmit_blynk_code = 1;
    Transmit_Blynk_Data_to_Mega();
}

void Going_Home()
{
    Serial.println("");
    Serial.println("Go to Dock");
    transmit_blynk_code = 2;
    Transmit_Blynk_Data_to_Mega();
}

void StartMower()
{
    Serial.println("");
    Serial.println("Quick Start");
    Mower_Parked = 0;
    Update_Blynk_App_With_Status();
    transmit_blynk_code = 3;       // Code used to let MEGA know what to do
    Transmit_Blynk_Data_to_Mega(); // Transmits that code to the MEGA
}

void Exit_Dock()
{
    Serial.println("");
    Serial.println("Exit Dock");
    Mower_Docked = 0; // Forces the APP to show docked as false.
                      // Otherwise the APP needs to wait for the next update round
    Update_Blynk_App_With_Status();
    transmit_blynk_code = 4;
    Transmit_Blynk_Data_to_Mega();
}

void Set_To_Manuel_Mode()
{
    Serial.println("");
    Serial.println("Manuel Mode Selected");
    Update_Blynk_App_With_Status();
    transmit_blynk_code = 5;
    Transmit_Blynk_Data_to_Mega();
    Manuel_Mode = 1;
}

void Set_To_Automatic_Mode_Random()
{
    Serial.println("");
    Serial.println("Automatic Mode RANDOM Selected");
    Update_Blynk_App_With_Status();
    transmit_blynk_code = 6;
    Transmit_Blynk_Data_to_Mega();
    Automatic_Mode = 1;
}

void Set_To_Automatic_Mode_Spiral()
{
    Serial.println("");
    Serial.println("Automatic Mode SPIRAL Selected");
    Update_Blynk_App_With_Status();
    transmit_blynk_code = 11;
    Transmit_Blynk_Data_to_Mega();
    Automatic_Mode = 2;
}

void Set_To_Automatic_Mode_Parallel()
{
    Serial.println("");
    Serial.println("Automatic Mode PARALLEL Selected");
    Update_Blynk_App_With_Status();
    transmit_blynk_code = 12;
    Transmit_Blynk_Data_to_Mega();
    Automatic_Mode = 2;
}
