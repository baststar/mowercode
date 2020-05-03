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
int slowSpeedLeft = PWM_MAXSPEED_LEFT - PWM_SLOWSPEED_SUBSTRACTION;
int slowSpeedRight = PWM_MAXSPEED_RIGHT - PWM_SLOWSPEED_SUBSTRACTION;

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
        beforeMenuFSMEvent = currentFSMEvent;
        Trigger_FSM(FSMEVENT_FOLLOW_WIRE__TO__PARKED, currentFSMSequence);
    }
}

void followWire_on_enter() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FOLLOW WIRE             ");
    delay(500);
    lcd.clear();
    startTimeFollowWire = millis();
    currentTimeFollowWire = startTimeFollowWire;
    lastTimeFollowWire = startTimeFollowWire;
    onSameSiteMillis = 0;
    motorLeftSpeed = 0;
    motorRightSpeed = 0;
    MotorAction_SetPinsToGoForward();
    UpdatePerimeterStatus();
    lastInside = MowerIsInsideWire();
}

void followWire() {

    lcd.setCursor(0, 0);
    lcd.print("followWire...                ");

    read_followWire_keys();

    UpdateVoltAmpCharge();
    if (IsCharging()) {
        Trigger_FSM(FSMEVENT_FOLLOW_WIRE__TO__DOCKED, -1);
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

        if (PERIMETER_IS_CLOCKWISE_FROM_GARAGE == true) {
            motorRightSpeed = slowSpeedRight;
            motorLeftSpeed = slowSpeedLeft - sFunction(magnitude, MAX_TRACKING_WIRE_MAGNITUDE_INSIDE, MAX_TRACKING_WIRE_MAGNITUDE_OUTSITE, slowSpeedLeft);

            if (motorLeftSpeed < 0) {
                motorLeftSpeed = 0;
            } else if (motorLeftSpeed > slowSpeedLeft) {
                motorLeftSpeed = slowSpeedLeft;
            }

            if (onSameSiteMillis > MAX_SAME_SITE_TRACKING_WIRE_TIME) {
                MotorAction_SetPinsToRotateLeft();
                motorLeftSpeed = slowSpeedLeft;
                motorRightSpeed = slowSpeedRight;
            } else {
                MotorAction_SetPinsToGoForward();
            }

        } else {

            motorLeftSpeed = slowSpeedLeft;
            motorRightSpeed = slowSpeedRight - sFunction(magnitude, MAX_TRACKING_WIRE_MAGNITUDE_INSIDE, MAX_TRACKING_WIRE_MAGNITUDE_OUTSITE, slowSpeedRight);
            if (motorRightSpeed < 0) {
                motorRightSpeed = 0;
            } else if (motorRightSpeed > slowSpeedRight) {
                motorRightSpeed = slowSpeedRight;
            }

            if (onSameSiteMillis > MAX_SAME_SITE_TRACKING_WIRE_TIME) {
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

        if (PERIMETER_IS_CLOCKWISE_FROM_GARAGE == true) {

            motorLeftSpeed = slowSpeedLeft;
            motorRightSpeed = slowSpeedRight - sFunction(magnitude, MAX_TRACKING_WIRE_MAGNITUDE_INSIDE, MAX_TRACKING_WIRE_MAGNITUDE_OUTSITE, slowSpeedRight);

            if (motorRightSpeed < 0) {
                motorRightSpeed = 0;
            } else if (motorRightSpeed > slowSpeedRight) {
                motorRightSpeed = slowSpeedRight;
            }

            if (onSameSiteMillis > MAX_SAME_SITE_TRACKING_WIRE_TIME) {
                MotorAction_SetPinsToRotateRight();
                motorLeftSpeed = slowSpeedLeft;
                motorRightSpeed = slowSpeedRight;
            } else {
                MotorAction_SetPinsToGoForward();
            }

        } else {

            motorRightSpeed = slowSpeedRight;
            motorLeftSpeed = slowSpeedLeft - sFunction(magnitude, MAX_TRACKING_WIRE_MAGNITUDE_INSIDE, MAX_TRACKING_WIRE_MAGNITUDE_OUTSITE, slowSpeedLeft);
            if (motorLeftSpeed < 0) {
                motorLeftSpeed = 0;
            } else if (motorLeftSpeed > slowSpeedLeft) {
                motorLeftSpeed = slowSpeedLeft;
            }

            if (onSameSiteMillis > MAX_SAME_SITE_TRACKING_WIRE_TIME) {
                MotorAction_SetPinsToRotateLeft();
                motorLeftSpeed = slowSpeedLeft;
                motorRightSpeed = slowSpeedRight;
            } else {
                MotorAction_SetPinsToGoForward();
            }
        }
    }

    if (currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_1) {
        if ((currentTimeFollowWire - startTimeFollowWire) >= FOLLOW_WIRE_ZONE_1_TIME) {
            Trigger_FSM(FSMEVENT_FOLLOW_WIRE__TO__WIRE_TO_GARDEN, currentFSMSequence);
        }
    } else if (currentFSMSequence == FSMSEQUENCE_EXIT_GARAGE_MOW_FROM_ZONE_2) {
        if ((currentTimeFollowWire - startTimeFollowWire) >= FOLLOW_WIRE_ZONE_2_TIME) {
            Trigger_FSM(FSMEVENT_FOLLOW_WIRE__TO__WIRE_TO_GARDEN, currentFSMSequence);
        }
    }

    MotorAction_GoPWMSpeed(motorLeftSpeed, motorRightSpeed);

    lastTimeFollowWire = currentTimeFollowWire;
}

void followWire_on_exit() {
    startTimeFollowWire = 0;
    currentTimeFollowWire = 0;
    MotorAction_StopMotors();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("EXIT FOLLOW WIRE           ");
    delay(3000);
}

State state_followWire(&followWire_on_enter, &followWire, &followWire_on_exit);
