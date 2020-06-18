#pragma once

void MotorAction_GoFullSpeed();
void MotorAction_GoSlowSpeed();
void MotorAction_GoPWMSpeed(int leftSpeed, int rightSpeed);
void MotorAction_StopMotors();
void MotorAction_SetPinsToGoForward();
void MotorAction_SetPinsToGoBackwards();
void MotorAction_SetPinsToRotateLeft();
void MotorAction_SetPinsToRotateRight();
void MotorAction_SetPinsToOnlyLeft();
void MotorAction_SetPinsToOnlyRight();


void MotorAction_StartBlades();
void MotorAction_StopBlades();

void RelaisOn();
void RelaisOff();
