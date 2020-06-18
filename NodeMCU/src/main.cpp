
/* Comment this out to disable prints and save space */

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.

#include <Arduino.h>

#include <BlynkSimpleEsp8266.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <SoftwareSerial.h>
#include <config.h>

#include <main.h>

#define ARRAY_SIZE(x) (int)(sizeof(x) / sizeof(x[0]))

#define BLYNK_PRINT Serial

//#define LED LED_BUILTIN
#define LED D4 // on various NODEMCU boards the LED Pin is different.....

SoftwareSerial NodeMCU(D2, D3);
BlynkTimer timer;
WidgetLCD lcd(V6);

const char* ssid = WLAN_SSID;
const char* password = WLAN_PASSWORD;
const char* mqtt_server = MQTT_HOST;
const int8_t mqtt_port = MQTT_PORT;

IPAddress staticIp(192, 168, 0, 230);
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192, 168, 0, 1);
IPAddress dns(192, 168, 0, 1);

static const char* cert_sh1_fingerprint PROGMEM = CERT_SHA1_FINGERPRINT;

BearSSL::WiFiClientSecure espClient;
MQTTClient mqttClient;
HTTPClient https;

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

// VoltSensor
float Volts;
float Volts_Last;
int Zero_Volts;
float vout = 0.0;
float R1 = 30000; // Mower 330 = 3000    Mower LAM = 30000
float R2 = 7500;  // Mower 330 = 7000

// AmpSensor
int Sensor = D7; // Der Stromstärkesensor wird am Pin A0 (Analog "0") angeschlossen.
int VpA = 185;   // Millivolt pro Ampere (100 für 20A Modul und 66 für 30A Modul)
int sensorwert = 0;
int Nullpunkt = 1600; // Spannung in mV bei dem keine Stromstärke vorhanden ist
double SensorSpannung = 0;
double Ampere = 0;

// RainSensor
int Rain_Detected;
bool raining = false;

// RawSensorData
unsigned long lastCheckedVoltAmpRain = 0;
int RawAmpere = 0;
int RawVolt = 0;
int RawRaining = 0;

// VOLT/AMP/RAIN PINS
#define VOLT_PIN A0
#define AMP_PIN D7
#define RAIN_PIN D6

void setup()
{
    Serial.begin(9600);
    NodeMCU.begin(9600);
    Serial.println();
    Serial.println("Setting up NODEMCU........");
    Serial.println("");
    pinMode(D2, INPUT);
    pinMode(D3, OUTPUT);
    pinMode(VOLT_PIN, INPUT);
    pinMode(AMP_PIN, INPUT);
    pinMode(RAIN_PIN, INPUT);

    digitalWrite(LED, HIGH); // Turn off LED Light
    Setup_Wifi();

    if (USE_BLYNK == 1) {
        Blynk_Connect(); // Connect to the WIFI
        Clear_APP();
    }

    lcd.clear();
    pinMode(LED, OUTPUT);

    if (USE_MQTT == 1) {
        espClient.setFingerprint(cert_sh1_fingerprint);
        mqttClient.begin(MQTT_HOST, MQTT_PORT, espClient);
        if (!mqttClient.connected()) {
            ConnectMQTT();
        }
    }
}

void loop()
{
    loopstatus = loopstatus + 1;
    RX_Data_Recieved = 0; // resets the data received bool

    if (USE_BLYNK == 1) {
        if (!Blynk.connected()) {
            Blynk.run();
            timer.run();
            digitalWrite(LED, HIGH);
            Serial.println("------------------------");
            Serial.println("NODEMCU Disconnected");
            Serial.println("Reconnecting ... ");
            Blynk_Connect();
        }
        else {
            Blynk.run();
            timer.run();
            digitalWrite(LED, LOW); // LED is inverted on a MODEMCU...
            Update_Blynk_App_With_Status();
            Serial.println(""); // new line serial monitor
        }
    }

    // Receive_All_From_MEGA();
    Receive_Mega_Topic();

    UpdateVoltAmpRain();
}

void UpdateVoltAmpRain()
{
    if (millis() - lastCheckedVoltAmpRain > 500) {
        lastCheckedVoltAmpRain = millis();

        RawAmpere = analogRead(AMP_PIN);
        RawVolt = analogRead(VOLT_PIN);
        RawRaining = analogRead(RAIN_PIN);

        // ADCMan.run();
        // if (ADCMan.isCaptureComplete(AMP_PIN)) {
        //     RawAmpere = ADCMan.read(AMP_PIN);
        // }

        // if (ADCMan.isCaptureComplete(VOLT_PIN)) {
        //     RawVolt = ADCMan.read(VOLT_PIN);
        // }

        // if (ADCMan.isCaptureComplete(RAIN_PIN)) {
        //     RawRaining = ADCMan.read(RAIN_PIN);
        // }

        UpdateVoltAmpCharge();
    }
}

void UpdateVoltAmpCharge()
{
    SensorSpannung = (RawAmpere / 1024.0) * 5000;  // Hier wird der Messwert in den Spannungswert am Sensor umgewandelt.
    Ampere = ((SensorSpannung - Nullpunkt) / VpA); // Im zweiten Schritt wird hier die Stromstärke berechnet.

    if (RawVolt > 100) {
        vout = (RawVolt * 5.0) / 1024.0; // see text
        Volts = vout / (R2 / (R1 + R2));
        Volts_Last = Volts;
        Zero_Volts = 0;
    }
    else {
        Volts = Volts_Last;
        Zero_Volts = Zero_Volts + 1;
        if (Zero_Volts > 5) {
            Volts = 0;
        }
    }

    if (RawRaining < 100) {
        raining = true;
    }
    else {
        raining = false;
    }

    Serial.println("new data: " + String(Ampere) + "  |  " + String(Volts) + "  |  " + String(raining));
}

#define INPUT_SIZE 30
char input[INPUT_SIZE + 1];

void Receive_Mega_Topic()
{
    while (NodeMCU.available()) {
        Serial.println("data available");

        byte size = NodeMCU.readBytes(input, INPUT_SIZE);
        input[size] = 0;

        Serial.println(input);
        char* command = strtok(input, ";");

        while (command != 0) {
            // Split the command in two values
            char* separator = strchr(command, '=');
            if (separator != 0) {
                // Actually split the string in 2: replace ':' with 0
                *separator = 0;
                int topic = atoi(command);
                ++separator;
                int value = atoi(separator);

                // Do something with servoId and position

                ProcessData(topic, value);
            }
            // Find the next command in input string
            command = strtok(0, ";");
        }
    }
}

String mowerStates[] = {"STATE_DOCKED",
                        "STATE_DOCKED_MENU",
                        "STATE_EXIT_GARAGE",
                        "STATE_MOWING",
                        "STATE_FOLLOW_WIRE",
                        "STATE_PARKED",
                        "STATE_PARKED_MENU",
                        "STATE_RANDOM_ROTATE",
                        "STATE_WIRE_TO_GARDEN",
                        "STATE_TEST_MENU",
                        "STATE_ERROR",
                        "STATE_COMPASS_ROTATE",
                        "STATE_ROTATE_TO_WIRE",
                        "STATE_FIND_WIRE_FORWARDS",
                        "STATE_FIND_WIRE_BACKWARDS",
                        "STATE_SETTINGS_MENU",
                        "STATE_SETTINGS_MOTORSPEED_MENU",
                        "STATE_SETTINGS_TIMES_MENU",
                        "STATE_SETTINGS_PERIMETER_MENU",
                        "STATE_SETTINGS_MOWTIMES_MENU",
                        "STATE_SETTINGS_ALARMS_MENU"};

void ProcessData(int topic, int value)
{
    Serial.println("topic:" + String(topic) + " value " + String(value));

    switch (topic) {
    case NodeMCUMessageTopics::NewState:

        if (ARRAY_SIZE(mowerStates) >= value) {
            mqttClient.publish("mower", mowerStates[value - 1], true, 3);
        }
        else {
            mqttClient.publish("mower", "unkown State", true, 3);
        }
        break;

    default:
        Serial.println("default");
        break;
    }
}

void myTimerEvent()
{
    if ((BatteryVoltage > 10) && (BatteryVoltage < 15)) {
        Blynk.virtualWrite(V3, BatteryVoltage);
    }

    Blynk.virtualWrite(V5, Loop_Cycle_Mowing);
}

void Setup_Wifi()
{
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.config(staticIp, dns, gateway, subnet);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    randomSeed(micros());
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void ConnectMQTT()
{
    Serial.println("Attempting MQTT connection...");
    while (!mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
        Serial.print("Attempting MQTT connection...");
        delay(5000);
    }
    // mqttClient.publish("/SmartHome/door/open", "1");
    mqttClient.publish("mower", "1");
}

// Functions
//***********************************

void Blynk_Connect()
{
    unsigned mytimeout = millis() / 1000;
    Serial.println();
    Serial.println("***************** BLYNC_CONNECT *****************");
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
        Blynk_Connect();
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
    // Print_RX_Values();
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
