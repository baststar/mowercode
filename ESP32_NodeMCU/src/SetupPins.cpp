
#include <Arduino.h>
#include <SetupPins.h>
#include <config.h>

void Setup_All_Pins() {
    Setup_Motor_Pins();
    Setup_Membrane_Button_Pins();
    Setup_Relay_Pins();
}

void Setup_Motor_Pins() {
    pinMode(PIN_ENA, OUTPUT); // Motor 1
    pinMode(PIN_IN_1, OUTPUT);
    pinMode(PIN_IN_2, OUTPUT);

    pinMode(PIN_ENB, OUTPUT); // Motor 2
    pinMode(PIN_IN_3, OUTPUT);
    pinMode(PIN_IN_4, OUTPUT);

    pinMode(PIN_L_EN, OUTPUT); // Blades
    pinMode(PIN_R_EN, OUTPUT);
    pinMode(PIN_RPWM, OUTPUT);
}

void Setup_Membrane_Button_Pins() {
    pinMode(PIN_START_KEY, INPUT_PULLUP); // set pin as input
    pinMode(PIN_PLUS_KEY, INPUT_PULLUP);  // set pin as input
    pinMode(PIN_MINUS_KEY, INPUT_PULLUP); // set pin as input
    pinMode(PIN_STOP_KEY, INPUT_PULLUP);  // set pin as input
}


void Setup_Relay_Pins() {
    pinMode(PIN_RELAY_MOTORS, OUTPUT); // Turn of the relay for the main battery power
    digitalWrite(PIN_RELAY_MOTORS, LOW);
}
