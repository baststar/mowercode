#include <Arduino.h>
#include <MotorActions.h>
#include <config.h>

bool isForwardMovement = true;

void MotorAction_GoFullSpeed() {

    int leftSpeed = PWM_MAXSPEED_LEFT;
    int rightSpeed = PWM_MAXSPEED_RIGHT;

    if (isForwardMovement == false) {
        int tmp = leftSpeed;
        leftSpeed = rightSpeed;
        rightSpeed = tmp;
    }

    analogWrite(PIN_ENA, rightSpeed);
    analogWrite(PIN_ENB, leftSpeed);
}

void MotorAction_GoSlowSpeed() {

    int leftSpeed = PWM_MAXSPEED_LEFT;
    int rightSpeed = PWM_MAXSPEED_RIGHT;

    if (isForwardMovement == false) {
        int tmp = leftSpeed;
        leftSpeed = rightSpeed;
        rightSpeed = tmp;
    }

    analogWrite(PIN_ENA, rightSpeed - PWM_SLOWSPEED_SUBSTRACTION);
    analogWrite(PIN_ENB, leftSpeed - PWM_SLOWSPEED_SUBSTRACTION);
}

void MotorAction_GoPWMSpeed(int leftSpeed, int rightSpeed) {

    if (isForwardMovement == false) {
        int tmp = leftSpeed;
        leftSpeed = rightSpeed;
        rightSpeed = tmp;
    }

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
    analogWrite(PIN_RPWM, PWM_BLADESPEED);
}

void MotorAction_StopBlades() {
    digitalWrite(PIN_R_EN, LOW);
    digitalWrite(PIN_L_EN, LOW);
    analogWrite(PIN_RPWM, 0);
}
