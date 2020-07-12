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

    digitalWrite(PIN_MOTOR_1_ENA, rightSpeed);
    digitalWrite(PIN_MOTOR_2_ENB, leftSpeed);
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

    digitalWrite(PIN_MOTOR_1_ENA, rightSlowSpeed);
    digitalWrite(PIN_MOTOR_2_ENB, leftSlowSpeed);
}

void MotorAction_GoPWMSpeed(int leftSpeed, int rightSpeed) {

    if (isForwardMovement == false) {
        int tmp = leftSpeed;
        leftSpeed = rightSpeed;
        rightSpeed = tmp;
    }

    digitalWrite(PIN_MOTOR_1_ENA, rightSpeed);
    digitalWrite(PIN_MOTOR_2_ENB, leftSpeed);
}

void MotorAction_StopMotors() {
    digitalWrite(PIN_MOTOR_1_ENA, 0); // Motor 1
    digitalWrite(PIN_MOTOR_1_IN_1, LOW);
    digitalWrite(PIN_MOTOR_1_IN_2, LOW);
    digitalWrite(PIN_MOTOR_2_ENB, 0); // Motor 2
    digitalWrite(PIN_MOTOR_2_IN_1, LOW);
    digitalWrite(PIN_MOTOR_2_IN_2, LOW);
}

void MotorAction_SetPinsToGoForward() {
    isForwardMovement = true;
    digitalWrite(PIN_MOTOR_1_IN_1, HIGH); // Motor 1
    digitalWrite(PIN_MOTOR_1_IN_2, LOW);
    digitalWrite(PIN_MOTOR_2_IN_1, HIGH); // Motor 2
    digitalWrite(PIN_MOTOR_2_IN_2, LOW);
}

void MotorAction_SetPinsToGoBackwards() {
    isForwardMovement = false;
    digitalWrite(PIN_MOTOR_1_IN_1, LOW); // Motor 1
    digitalWrite(PIN_MOTOR_1_IN_2, HIGH);
    digitalWrite(PIN_MOTOR_2_IN_1, LOW); // Motor 2
    digitalWrite(PIN_MOTOR_2_IN_2, HIGH);
}

void MotorAction_SetPinsToRotateLeft() {
    digitalWrite(PIN_MOTOR_1_IN_1, LOW); // Motor 1
    digitalWrite(PIN_MOTOR_1_IN_2, HIGH);
    digitalWrite(PIN_MOTOR_2_IN_1, HIGH); // Motor 2
    digitalWrite(PIN_MOTOR_2_IN_2, LOW);
}

void MotorAction_SetPinsToOnlyLeft() {
    digitalWrite(PIN_MOTOR_1_IN_1, LOW); // Motor 1
    digitalWrite(PIN_MOTOR_1_IN_2, LOW);
    digitalWrite(PIN_MOTOR_2_IN_1, LOW); // Motor 2
    digitalWrite(PIN_MOTOR_2_IN_2, HIGH);
}

void MotorAction_SetPinsToRotateRight() {
    digitalWrite(PIN_MOTOR_1_IN_1, HIGH); // Motor 1
    digitalWrite(PIN_MOTOR_1_IN_2, LOW);
    digitalWrite(PIN_MOTOR_2_IN_1, LOW); // Motor 2
    digitalWrite(PIN_MOTOR_2_IN_2, HIGH);
}
void MotorAction_SetPinsToOnlyRight() {
    digitalWrite(PIN_MOTOR_1_IN_1, LOW); // Motor 1
    digitalWrite(PIN_MOTOR_1_IN_2, HIGH);
    digitalWrite(PIN_MOTOR_2_IN_1, LOW); // Motor 2
    digitalWrite(PIN_MOTOR_2_IN_2, LOW);
}

void MotorAction_StartBlades() {
    digitalWrite(PIN_BLADES_R_EN, HIGH);
    digitalWrite(PIN_BLADES_L_EN, HIGH);
    digitalWrite(PIN_BLADES_RPWM, eeprom_pwm_bladespeed);
}

void MotorAction_StopBlades() {
    digitalWrite(PIN_BLADES_R_EN, LOW);
    digitalWrite(PIN_BLADES_L_EN, LOW);
    digitalWrite(PIN_BLADES_RPWM, 0);
}

void RelaisOn() {
    digitalWrite(PIN_RELAY_MOTORS, HIGH);
}

void RelaisOff() {
    digitalWrite(PIN_RELAY_MOTORS, LOW);
}
