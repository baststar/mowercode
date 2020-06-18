
#include "../.pio/libdeps/megaatmega2560/Ardumower/code/ardumower/adcman.h"
#include <Arduino.h>
#include <SetupPins.h>
#include <config.h>



void Setup_All_Pins() {
    Setup_Motor_Pins();
    Setup_Membrane_Button_Pins();
    Setup_Bumper_Bar_Pins();
    Setup_Relay_Pins();
    Setup_VoltAmpRain_Pins();
}

void Setup_Motor_Pins() {
    pinMode(PIN_L_EN, OUTPUT);
    pinMode(PIN_R_EN, OUTPUT);
    pinMode(PIN_RPWM, OUTPUT);
}

void Setup_Membrane_Button_Pins() {
    pinMode(PIN_START_KEY, INPUT_PULLUP); // set pin as input
    pinMode(PIN_PLUS_KEY, INPUT_PULLUP);  // set pin as input
    pinMode(PIN_MINUS_KEY, INPUT_PULLUP); // set pin as input
    pinMode(PIN_STOP_KEY, INPUT_PULLUP);  // set pin as input
}

void Setup_Bumper_Bar_Pins() {
    pinMode(BUMPER_SWITCH_FRONT_RH, INPUT_PULLUP);
    pinMode(BUMPER_SWITCH_FRONT_LH, INPUT_PULLUP);
}

void Setup_Relay_Pins() {
    pinMode(PIN_RELAY_MOTORS, OUTPUT); // Turn of the relay for the main battery power
}

void Setup_VoltAmpRain_Pins() {
    
}
