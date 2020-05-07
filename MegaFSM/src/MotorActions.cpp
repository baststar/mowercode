#include <Arduino.h>
#include <EEPROMVariables.h>
#include <MotorActions.h>
#include <config.h>


bool isForwardMovement = true;

void MotorAction_GoFullSpeed() {

    int leftSpeed = eeprom_pwm_maxspeed_left;
    int rightSpeed = eeprom_pwm_maxspeed_right;

    if (isForwardMovement == false) {
        int tmp = leftSpeed;
        leftSpeed = rightSpeed;
        rightSpeed = tmp;
    }

    analogWrite(PIN_ENA, rightSpeed);
    analogWrite(PIN_ENB, leftSpeed);
}

void MotorAction_GoSlowSpeed() {

    int leftSpeed = eeprom_pwm_maxspeed_left;
    int rightSpeed = eeprom_pwm_maxspeed_right;

    if (isForwardMovement == false) {
        int tmp = leftSpeed;
        leftSpeed = rightSpeed;
        rightSpeed = tmp;
    }

    int rightSlowSpeed = eeprom_pwm_slowspeed - (255 - rightSpeed);
    int leftSlowSpeed = eeprom_pwm_slowspeed - (255 - leftSpeed);

    analogWrite(PIN_ENA, rightSlowSpeed);
    analogWrite(PIN_ENB, leftSlowSpeed);
}

void MotorAction_GoPWMSpeed(int leftSpeed, int rightSpeed) {

    if (isForwardMovement == false) {
        int tmp = leftSpeed;
        leftSpeed = rightSpeed;
        rightSpeed = tmp;
    }

    analogWrite(PIN_ENA, rightSpeed);
    analogWrite(PIN_ENB, leftSpeed);
}

void MotorAction_StopMotors() {
    digitalWrite(PIN_ENA, 0); // Motor 1
    digitalWrite(PIN_IN_1, LOW);
    digitalWrite(PIN_IN_2, LOW);
    digitalWrite(PIN_ENB, 0); // Motor 2
    digitalWrite(PIN_IN_3, LOW);
    digitalWrite(PIN_IN_4, LOW);
}

void MotorAction_SetPinsToGoForward() {
    isForwardMovement = true;
    digitalWrite(PIN_IN_1, LOW); // Motor 1
    digitalWrite(PIN_IN_2, HIGH);
    digitalWrite(PIN_IN_3, LOW); // Motor 2
    digitalWrite(PIN_IN_4, HIGH);
}

void MotorAction_SetPinsToGoBackwards() {
    isForwardMovement = false;
    digitalWrite(PIN_IN_1, HIGH); // Motor 1
    digitalWrite(PIN_IN_2, LOW);
    digitalWrite(PIN_IN_3, HIGH); // Motor 2
    digitalWrite(PIN_IN_4, LOW);
}

void MotorAction_SetPinsToRotateLeft() {
    digitalWrite(PIN_IN_1, LOW); // Motor 1
    digitalWrite(PIN_IN_2, HIGH);
    digitalWrite(PIN_IN_3, HIGH); // Motor 2
    digitalWrite(PIN_IN_4, LOW);
}

void MotorAction_SetPinsToRotateRight() {
    digitalWrite(PIN_IN_1, HIGH); // Motor 1
    digitalWrite(PIN_IN_2, LOW);
    digitalWrite(PIN_IN_3, LOW); // Motor 2
    digitalWrite(PIN_IN_4, HIGH);
}

void MotorAction_StartBlades() {
    digitalWrite(PIN_R_EN, HIGH);
    digitalWrite(PIN_L_EN, HIGH);
    analogWrite(PIN_RPWM, eeprom_pwm_bladespeed);
}

void MotorAction_StopBlades() {
    digitalWrite(PIN_R_EN, LOW);
    digitalWrite(PIN_L_EN, LOW);
    analogWrite(PIN_RPWM, 0);
}
