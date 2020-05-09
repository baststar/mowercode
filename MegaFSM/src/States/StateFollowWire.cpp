#include <EEPROMVariables.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <Serial1.h>
#include <States/FSMEvents.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/StateFollowWire.h>
#include <config.h>
#include <perimeter.h>



long int startTimeFollowWire = 0;
long int currentTimeFollowWire = 0;
long int lastTimeFollowWire = 0;
bool dockedFollowWire = false;
bool lastInside = false;
long int onSameSiteMillis = 0;
int motorLeftSpeed = 0;
int motorRightSpeed = 0;


int slowSpeedRight = eeprom_pwm_slowspeed - (255 - eeprom_pwm_maxspeed_right);
int slowSpeedLeft = eeprom_pwm_slowspeed - (255 - eeprom_pwm_maxspeed_left);

int sFunction(int magnitude, int maxMagnitudeInside, int maxMagnitudeOutside, int maxSpeed) {

    double cappedMagnitude = abs(magnitude);
    float percent = 100;
    if (magnitude > 0) {

        // outside from wire // right from wire
        if (cappedMagnitude > maxMagnitudeOutside) {
            cappedMagnitude = maxMagnitudeOutside;
        }
        percent = (cappedMagnitude / maxMagnitudeOutside) * 100;

    } else if (magnitude < 0) {
        // inside from wire // left from wire
        if (cappedMagnitude > maxMagnitudeInside) {
            cappedMagnitude = maxMagnitudeInside;
        }
        percent = (cappedMagnitude / maxMagnitudeInside) * 100;
    }
    return int(percent * maxSpeed / 100);
}

void read_followWire_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMEvent = currentFSMEvent;
        TriggerFSM(STATE_FOLLOW_WIRE, STATE_PARKED, currentFSMSequence);
        return;
    }
}

void followWire_on_enter() {
    clearLCD();
    lcd.setCursor(0, 0);
    lcd.print("FOLLOW WIRE             ");
    delay(2000);
    clearLCD();
    startTimeFollowWire = millis();
    currentTimeFollowWire = startTimeFollowWire;
    lastTimeFollowWire = startTimeFollowWire;
    onSameSiteMillis = 0;
    motorLeftSpeed = 0;
    motorRightSpeed = 0;
    UpdatePerimeterStatus();
    lastInside = MowerIsInsideWire();
    MotorAction_SetPinsToGoForward();
}

void followWire() {

    lcd.setCursor(0, 0);
    lcd.print("followWire                ");

    read_followWire_keys();

    UpdateVoltAmpCharge();
    if (IsCharging()) {
        MotorAction_StopMotors();
        lcd.setCursor(0, 0);
        lcd.print("FOUND CHARGING");
        delay(5000);
        TriggerFSM(STATE_FOLLOW_WIRE, STATE_DOCKED, -1);
        return;
    }

    UpdatePerimeterStatus();

    int magnitude = GetCurrentMagnitude();
    bool inside = MowerIsInsideWire();
    currentTimeFollowWire = millis();

    if (magnitude > 0) {

        if (lastInside != inside) {
            onSameSiteMillis = 0;
            lastInside = inside;
        } else {
            onSameSiteMillis = onSameSiteMillis + (currentTimeFollowWire - lastTimeFollowWire);
        }

        if (eeprom_perimeter_is_clockwise_from_garage == 1) {
            motorRightSpeed = slowSpeedRight;
            motorLeftSpeed = slowSpeedLeft - sFunction(magnitude, eeprom_max_tracking_wire_magnitude_inside, eeprom_max_tracking_wire_magnitude_outside, slowSpeedLeft);

            if (motorLeftSpeed < 0) {
                motorLeftSpeed = 0;
            } else if (motorLeftSpeed > slowSpeedLeft) {
                motorLeftSpeed = slowSpeedLeft;
            }

            if (onSameSiteMillis > eeprom_max_same_side_tracking_wire_time) {
                MotorAction_SetPinsToRotateLeft();
                motorLeftSpeed = slowSpeedLeft;
                motorRightSpeed = slowSpeedRight;
            } else {
                MotorAction_SetPinsToGoForward();
            }

        } else {

            motorLeftSpeed = slowSpeedLeft;
            motorRightSpeed = slowSpeedRight - sFunction(magnitude, eeprom_max_tracking_wire_magnitude_inside, eeprom_max_tracking_wire_magnitude_outside, slowSpeedRight);
            if (motorRightSpeed < 0) {
                motorRightSpeed = 0;
            } else if (motorRightSpeed > slowSpeedRight) {
                motorRightSpeed = slowSpeedRight;
            }

            if (onSameSiteMillis > eeprom_max_same_side_tracking_wire_time) {
                MotorAction_SetPinsToRotateRight();
                motorLeftSpeed = slowSpeedLeft;
                motorRightSpeed = slowSpeedRight;
            } else {
                MotorAction_SetPinsToGoForward();
            }
        }

    } else if (magnitude < 0) {

        if (lastInside != inside) {
            onSameSiteMillis = 0;
            lastInside = inside;
        } else {
            onSameSiteMillis = onSameSiteMillis + (currentTimeFollowWire - lastTimeFollowWire);
        }

        if (eeprom_perimeter_is_clockwise_from_garage == 1) {

            motorLeftSpeed = slowSpeedLeft;
            motorRightSpeed = slowSpeedRight - sFunction(magnitude, eeprom_max_tracking_wire_magnitude_inside, eeprom_max_tracking_wire_magnitude_outside, slowSpeedRight);

            if (motorRightSpeed < 0) {
                motorRightSpeed = 0;
            } else if (motorRightSpeed > slowSpeedRight) {
                motorRightSpeed = slowSpeedRight;
            }

            if (onSameSiteMillis > eeprom_max_same_side_tracking_wire_time) {
                MotorAction_SetPinsToRotateRight();
                motorLeftSpeed = slowSpeedLeft;
                motorRightSpeed = slowSpeedRight;
            } else {
                MotorAction_SetPinsToGoForward();
            }

        } else {

            motorRightSpeed = slowSpeedRight;
            motorLeftSpeed = slowSpeedLeft - sFunction(magnitude, eeprom_max_tracking_wire_magnitude_inside, eeprom_max_tracking_wire_magnitude_outside, slowSpeedLeft);
            if (motorLeftSpeed < 0) {
                motorLeftSpeed = 0;
            } else if (motorLeftSpeed > slowSpeedLeft) {
                motorLeftSpeed = slowSpeedLeft;
            }

            if (onSameSiteMillis > eeprom_max_same_side_tracking_wire_time) {
                MotorAction_SetPinsToRotateLeft();
                motorLeftSpeed = slowSpeedLeft;
                motorRightSpeed = slowSpeedRight;
            } else {
                MotorAction_SetPinsToGoForward();
            }
        }
    }

    if (currentFSMSequence == FSMSEQUENCE_ZONE_1) {
        if ((currentTimeFollowWire - startTimeFollowWire) >= eeprom_follow_wire_zone_1_time) {
            TriggerFSM(STATE_FOLLOW_WIRE, STATE_WIRE_TO_GARDEN, currentFSMSequence);
            return;
        }
    } else if (currentFSMSequence == FSMSEQUENCE_ZONE_2) {
        if ((currentTimeFollowWire - startTimeFollowWire) >= eeprom_follow_wire_zone_2_time) {
            TriggerFSM(STATE_FOLLOW_WIRE, STATE_WIRE_TO_GARDEN, currentFSMSequence);
            return;
        }
    }

    MotorAction_GoPWMSpeed(motorLeftSpeed, motorRightSpeed);

    lastTimeFollowWire = currentTimeFollowWire;
}

void followWire_on_exit() {
    MotorAction_StopMotors();
}

State state_followWire(&followWire_on_enter, &followWire, &followWire_on_exit);
