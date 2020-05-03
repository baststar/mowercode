#include <Arduino.h>
#include <MotorActions.h>
#include <config.h>


void MotorAction_GoFullSpeed() {
    analogWrite(PIN_ENA, PWM_MAXSPEED_RIGHT);
    analogWrite(PIN_ENB, PWM_MAXSPEED_LEFT);
}

void MotorAction_GoSlowSpeed() {
    analogWrite(PIN_ENA, PWM_MAXSPEED_RIGHT - PWM_SLOWSPEED_SUBSTRACTION);
    analogWrite(PIN_ENB, PWM_MAXSPEED_LEFT - PWM_SLOWSPEED_SUBSTRACTION);
}

void MotorAction_GoPWMSpeed(int leftSpeed, int rightSpeed) {
    analogWrite(PIN_ENA, rightSpeed); // ENA low = Right Swerve   ENB low = Left Swerve
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
    digitalWrite(PIN_IN_1, LOW); // Motor 1
    digitalWrite(PIN_IN_2, HIGH);
    digitalWrite(PIN_IN_3, LOW); // Motor 2
    digitalWrite(PIN_IN_4, HIGH);
}

void MotorAction_SetPinsToGoBackwards() {
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
    analogWrite(PIN_RPWM, PWM_BLADESPEED);
}

void MotorAction_StopBlades() {
    digitalWrite(PIN_R_EN, LOW);
    digitalWrite(PIN_L_EN, LOW);
    analogWrite(PIN_RPWM, 0);
}
