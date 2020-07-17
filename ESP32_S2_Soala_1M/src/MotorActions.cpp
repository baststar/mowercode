#include <Arduino.h>
#include <EEPROMVariables.h>
#include <MotorActions.h>
#include <SetupPins.h>
#include <config.h>
#include <driver/adc.h>
#include <driver/gpio.h>
#include <driver/ledc.h>


bool isForwardMovement = true;

void MotorAction_GoFullSpeed() {

    int leftSpeed = eeprom_pwm_maxspeed_left;
    int rightSpeed = eeprom_pwm_maxspeed_right;

    if (isForwardMovement == false) {
        int tmp = leftSpeed;
        leftSpeed = rightSpeed;
        rightSpeed = tmp;
    }

    ledc_set_duty(ledc_channel_motor_1.speed_mode, ledc_channel_motor_1.channel, rightSpeed);
    ledc_update_duty(ledc_channel_motor_1.speed_mode, ledc_channel_motor_1.channel);

    ledc_set_duty(ledc_channel_motor_2.speed_mode, ledc_channel_motor_2.channel, leftSpeed);
    ledc_update_duty(ledc_channel_motor_2.speed_mode, ledc_channel_motor_2.channel);
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

    ledc_set_duty(ledc_channel_motor_1.speed_mode, ledc_channel_motor_1.channel, rightSlowSpeed);
    ledc_update_duty(ledc_channel_motor_1.speed_mode, ledc_channel_motor_1.channel);

    ledc_set_duty(ledc_channel_motor_2.speed_mode, ledc_channel_motor_2.channel, leftSlowSpeed);
    ledc_update_duty(ledc_channel_motor_2.speed_mode, ledc_channel_motor_2.channel);
}

void MotorAction_GoPWMSpeed(int leftSpeed, int rightSpeed) {

    if (isForwardMovement == false) {
        int tmp = leftSpeed;
        leftSpeed = rightSpeed;
        rightSpeed = tmp;
    }

    ledc_set_duty(ledc_channel_motor_1.speed_mode, ledc_channel_motor_1.channel, rightSpeed);
    ledc_update_duty(ledc_channel_motor_1.speed_mode, ledc_channel_motor_1.channel);

    ledc_set_duty(ledc_channel_motor_2.speed_mode, ledc_channel_motor_2.channel, leftSpeed);
    ledc_update_duty(ledc_channel_motor_2.speed_mode, ledc_channel_motor_2.channel);
}

void MotorAction_StopMotors() {

    ledc_set_duty(ledc_channel_motor_1.speed_mode, ledc_channel_motor_1.channel, 0);
    ledc_update_duty(ledc_channel_motor_1.speed_mode, ledc_channel_motor_1.channel);

    ledc_set_duty(ledc_channel_motor_2.speed_mode, ledc_channel_motor_2.channel, 0);
    ledc_update_duty(ledc_channel_motor_2.speed_mode, ledc_channel_motor_2.channel);

    digitalWrite(PIN_MOTOR_1_IN_1, LOW);
    digitalWrite(PIN_MOTOR_1_IN_2, LOW);

    digitalWrite(PIN_MOTOR_2_IN_1, LOW);
    digitalWrite(PIN_MOTOR_2_IN_2, LOW);
}

void MotorAction_SetPinsToGoForward() {

    isForwardMovement = true;
    digitalWrite(PIN_MOTOR_1_IN_1, LOW); // Motor 1
    digitalWrite(PIN_MOTOR_1_IN_2, HIGH);
    digitalWrite(PIN_MOTOR_2_IN_1, LOW); // Motor 2
    digitalWrite(PIN_MOTOR_2_IN_2, HIGH);
}

void MotorAction_SetPinsToGoBackwards() {

    isForwardMovement = false;
    digitalWrite(PIN_MOTOR_1_IN_1, HIGH); // Motor 1
    digitalWrite(PIN_MOTOR_1_IN_2, LOW);
    digitalWrite(PIN_MOTOR_2_IN_1, HIGH); // Motor 2
    digitalWrite(PIN_MOTOR_2_IN_2, LOW);
}

void MotorAction_SetPinsToRotateLeft() {

    digitalWrite(PIN_MOTOR_1_IN_1, LOW); // Motor 1
    digitalWrite(PIN_MOTOR_1_IN_2, HIGH);
    digitalWrite(PIN_MOTOR_2_IN_1, HIGH); // Motor 2
    digitalWrite(PIN_MOTOR_2_IN_2, LOW);
}

void MotorAction_SetPinsToOnlyLeft() {

    digitalWrite(PIN_MOTOR_1_IN_1, LOW); // Motor 1
    digitalWrite(PIN_MOTOR_1_IN_2, HIGH);
    digitalWrite(PIN_MOTOR_2_IN_1, LOW); // Motor 2
    digitalWrite(PIN_MOTOR_2_IN_2, LOW);
}

void MotorAction_SetPinsToRotateRight() {

    digitalWrite(PIN_MOTOR_1_IN_1, HIGH); // Motor 1
    digitalWrite(PIN_MOTOR_1_IN_2, LOW);
    digitalWrite(PIN_MOTOR_2_IN_1, LOW); // Motor 2
    digitalWrite(PIN_MOTOR_2_IN_2, HIGH);
}
void MotorAction_SetPinsToOnlyRight() {

    digitalWrite(PIN_MOTOR_1_IN_1, LOW); // Motor 1
    digitalWrite(PIN_MOTOR_1_IN_2, LOW);
    digitalWrite(PIN_MOTOR_2_IN_1, LOW); // Motor 2
    digitalWrite(PIN_MOTOR_2_IN_2, HIGH);
}

void MotorAction_StartBlades() {

    digitalWrite(PIN_BLADES_R_EN, HIGH);
    digitalWrite(PIN_BLADES_L_EN, HIGH);

    ledc_set_duty(ledc_channel_blades.speed_mode, ledc_channel_blades.channel, eeprom_pwm_bladespeed);
    ledc_update_duty(ledc_channel_blades.speed_mode, ledc_channel_blades.channel);
}

void MotorAction_StopBlades() {

    digitalWrite(PIN_BLADES_R_EN, LOW);
    digitalWrite(PIN_BLADES_L_EN, LOW);

    ledc_set_duty(ledc_channel_blades.speed_mode, ledc_channel_blades.channel, 0);
    ledc_update_duty(ledc_channel_blades.speed_mode, ledc_channel_blades.channel);
}

void RelaisOn() {
    digitalWrite(PIN_RELAY_MOTORS, HIGH);
}

void RelaisOff() {
    digitalWrite(PIN_RELAY_MOTORS, LOW);
}
