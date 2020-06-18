#include <EEPROMVariables.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <MotorActions.h>
#include <Perimeter.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/FSMStates.h>
#include <States/StateFollowWire.h>
#include <VoltAmpRain.h>
#include <config.h>


unsigned long startTimeFollowWire = 0;
unsigned long lastTimeFollowWire = 0;
bool dockedFollowWire = false;
bool lastInside = false;
unsigned long onSameSiteMillis = 0;
unsigned long onSameSiteMillisShortMax = 2000;
unsigned long onOtherSiteMillis = 0;
int motorLeftSpeed = 0;
int motorRightSpeed = 0;

int slowSpeedRight = eeprom_pwm_slowspeed - (255 - eeprom_pwm_maxspeed_right);
int slowSpeedLeft = eeprom_pwm_slowspeed - (255 - eeprom_pwm_maxspeed_left);

int sFunction(int magnitude, int maxMagnitudeInside, int maxMagnitudeOutside, int maxSpeed, unsigned long onSameSide, unsigned long onOtherSide, unsigned long timeMax) {

    if (onSameSide > timeMax) {
        onSameSide = timeMax;
    }

    float timePercentSameSide = (((100.0 / timeMax) * onSameSide) / 100.0);
    float timePercentOtherSide = (((100.0 / timeMax) * onOtherSide) / 100.0);

    double cappedMagnitude = abs(magnitude);
    float percent = 100;

    if (magnitude > 0) {

        // outside from wire // right from wire
        if (cappedMagnitude > maxMagnitudeOutside) {
            cappedMagnitude = maxMagnitudeOutside;
        }
        percent = (cappedMagnitude / maxMagnitudeOutside) * 100.0;

    } else if (magnitude < 0) {

        // inside from wire // left from wire
        if (cappedMagnitude > maxMagnitudeInside) {
            cappedMagnitude = maxMagnitudeInside;
        }
        percent = (cappedMagnitude / maxMagnitudeInside) * 100.0;
    }

    int resultSpeedSubstraction = int(percent * maxSpeed / 100.0 * timePercentSameSide) - int(timePercentOtherSide * maxSpeed);


    lcd.setCursor(0, 0);
    lcd.print(String(timePercentSameSide) + " " + String(percent) + "      ");
    lcd.setCursor(0, 1);
    lcd.print(String(resultSpeedSubstraction) + " m " + String(magnitude) + "      ");

    // return resultSpeedSubstraction;
    return resultSpeedSubstraction;
}

struct speedCalculationResult {
    int leftSpeed;
    int rightSpeed;
};

int mowerSide = 0; // -1 left 0 middle 1 right
int lastMowerSide = 0;
unsigned long timeOnSameSide = 0;
unsigned long timeLastCheck = 0;

speedCalculationResult calculateMotorspeed() {

    int linksUeberKabel = -200;
    int linksRechtsUeberKabel = -1000;
    int linksMitte = -900;

    int rechtsUeberKabel = -500;
    int rechtsLinksUeberKabel = 850;
    int rechtsMitte = 600;

    int magnitudeRight = GetCurrentMagnitudeRight();
    int magnitudeLeft = GetCurrentMagnitudeLeft();

    int absMagnitudeRight = abs(magnitudeRight);
    int absMagnitudeLeft = abs(absMagnitudeLeft);
    // int maxRightMagnitude = eeprom_max_tracking_wire_magnitude_outside;
    // int maxLeftMagnitude = eeprom_max_tracking_wire_magnitude_inside;

    int leftSpeed = slowSpeedLeft;
    int rightSpeed = slowSpeedRight;

    lcd.setCursor(0, 0);
    lcd.print("mLeft " + String(magnitudeLeft) + "           ");
    lcd.setCursor(0, 1);
    lcd.print("mRight " + String(magnitudeRight) + "         ");


    if (magnitudeLeft > linksRechtsUeberKabel && magnitudeLeft < linksUeberKabel && magnitudeRight < rechtsLinksUeberKabel && magnitudeRight > rechtsUeberKabel) {

        mowerSide = 0;

        // beide innen, theoretisch gradeaus

        // bisschen zu weit links:
        // rechts: 200 -> muss zu 850
        // links: -950 -> muss zu -1000

        // bisschen zu weit rechts:
        // rechts: 750 -> muss zu 850
        // links: -800 -> muss zu -1000

        leftSpeed = slowSpeedLeft;
        rightSpeed = slowSpeedRight;

    } else if (magnitudeLeft > linksUeberKabel) {
        // beide rechts vom kabel CW
        // mitte zwischen den kabeln -> links langsamer

        mowerSide = 1;

        leftSpeed = 0;

    } else if (magnitudeRight < rechtsUeberKabel) {
        // beide links vom kabel CW
        // mitte zwischen den kabeln - rechts langsamer

        mowerSide = -1;

        rightSpeed = 0;

        //     -95             -1000                    200              850
    }

    if (mowerSide != 0) {

        if (lastMowerSide != mowerSide) {
        }

        if (lastMowerSide == mowerSide) {

            timeOnSameSide += millis() - timeLastCheck;
        }
    }

    timeLastCheck = millis();

    return speedCalculationResult{leftSpeed, rightSpeed};
}


void followFunction() {

    int magnitude = GetCurrentMagnitudeLeft();
    bool inside = MowerIsInsideWire();

    if (inside == lastInside) {

        onSameSiteMillis = onSameSiteMillis + (millis() - lastTimeFollowWire);

        if (onSameSiteMillis > eeprom_max_same_side_tracking_wire_time) {

            if (inside == false) {
                TriggerFSM(currentState, STATE_FIND_WIRE_BACKWARDS, currentFSMSequence);
                return;
            } else {
                // backwards, random rotate, findWireForwards, continue...
                MotorAction_StopMotors();
                delay(500);
                MotorAction_SetPinsToGoBackwards();
                MotorAction_GoSlowSpeed();
                delay(1500);
                TriggerFSM(currentState, STATE_RANDOM_ROTATE, currentFSMSequence);
                return;
            }
        }

        if (onSameSiteMillis > onSameSiteMillisShortMax) {

            if (eeprom_perimeter_is_clockwise_from_garage == 1) {

                if (inside) {
                    // TriggerFSM(currentState, STATE_FIND_WIRE_FORWARDS, currentFSMSequence);
                    // return;
                    MotorAction_SetPinsToOnlyRight();
                } else {
                    MotorAction_SetPinsToRotateLeft();
                }

            } else {
                if (inside) {
                    MotorAction_SetPinsToRotateLeft();
                } else {
                    MotorAction_SetPinsToOnlyRight();
                }
            }
        }

    } else {
        lastInside = inside;
        onSameSiteMillis = 0;
    }

    int speedLeft = 255;
    int speedRight = 255;

    if (eeprom_perimeter_is_clockwise_from_garage == 1) {

        // if (inside == true) {
        // anhand der magnitude * p links bisschen weniger speed
        // magnitude ist z.B. -1000 eeprom_max_tracking_wire_magnitude_inside

        int mag_error = 0 - magnitude; // - - = +

        if (mag_error > 0) {

            // right turn
            speedLeft = 255;
            speedRight = 255 - (mag_error * (eeprom_p_value / 100.0));
            if (speedRight > 255) {
                speedRight = 255;
            }
            if (speedRight >= 0) {
                MotorAction_SetPinsToGoForward();
            }
            if (speedRight < 0) {
                speedRight = (-1 * speedRight) + 220;
                if (speedRight > 255) {
                    speedRight = 255;
                }
                MotorAction_SetPinsToRotateRight();
            }

        } else if (mag_error <= 0) {

            // left turn
            speedRight = 255;
            speedLeft = 255 + (mag_error * (eeprom_p_value / 100.0));
            if (speedLeft > 255) {
                speedLeft = 255;
            }
            if (speedLeft >= 0) {
                MotorAction_SetPinsToGoForward();
            }
            if (speedLeft < 0) {
                speedLeft = (-1 * speedLeft) + 220;
                if (speedLeft > 255) {
                    speedLeft = 255;
                }
                MotorAction_SetPinsToRotateLeft();
            }
        }
        //}
    } else {
        // ccw ...
    }


    lcd.setCursor(0, 0);
    lcd.print(String(speedLeft) + "   " + String(speedRight) + "        ");

    speedCalculationResult speedResult = {speedLeft, speedRight};

    MotorAction_GoPWMSpeed(speedResult.leftSpeed, speedResult.rightSpeed);

    lastTimeFollowWire = millis();
}


void read_followWire_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);
        beforeMenuFSMTransition = currentFSMTransition;
        TriggerFSM(STATE_FOLLOW_WIRE, STATE_PARKED, currentFSMSequence);
        return;
    }
}

void followWire_on_enter() {
    RelaisOn();
    clearLCD();
    startTimeFollowWire = millis();
    lastTimeFollowWire = startTimeFollowWire;
    onSameSiteMillis = 0;
    motorLeftSpeed = 0;
    motorRightSpeed = 0;
    lastInside = MowerIsInsideWire();
    MotorAction_SetPinsToGoForward();
}

void followWire() {

    // lcd.setCursor(0, 0);
    // lcd.print("followWire                ");

    if (IsCharging()) {
        MotorAction_StopMotors();
        TriggerFSM(STATE_FOLLOW_WIRE, STATE_DOCKED, -1);
        ShowError("CHARGING");
        return;
    }

    followFunction();

    // bool inside = MowerIsInsideWire();

    // int magnitude = GetCurrentMagnitudeLeft();

    // if (magnitude > 0) {

    //     if (lastInside != inside) {
    //         onSameSiteMillis = 0;
    //         lastInside = inside;
    //         onOtherSiteMillis += (currentMillisGlobal - lastTimeFollowWire);
    //     } else {
    //         onSameSiteMillis += (currentMillisGlobal - lastTimeFollowWire);
    //         onOtherSiteMillis = 0;
    //     }

    //     if (eeprom_perimeter_is_clockwise_from_garage == 1) {

    //         motorRightSpeed = slowSpeedRight;
    //         motorLeftSpeed = slowSpeedLeft - sFunction(magnitude, eeprom_max_tracking_wire_magnitude_inside, eeprom_max_tracking_wire_magnitude_outside, slowSpeedLeft, onSameSiteMillis,
    //                                                    onOtherSiteMillis, eeprom_max_same_side_tracking_wire_time);

    //         // lcd.setCursor(0, 0);
    //         // lcd.print("RightSpeed " + String(motorRightSpeed) + "         ");
    //         // lcd.setCursor(0, 1);
    //         // lcd.print("LeftSpeed " + String(motorLeftSpeed) + "         ");

    //         if (motorLeftSpeed < 0) {
    //             motorLeftSpeed = 0;
    //         } else if (motorLeftSpeed > slowSpeedLeft) {
    //             motorLeftSpeed = slowSpeedLeft;
    //         }

    //         // if (onSameSiteMillis > eeprom_max_same_side_tracking_wire_time) {
    //         //     MotorAction_SetPinsToRotateLeft();
    //         //     motorLeftSpeed = slowSpeedLeft;
    //         //     motorRightSpeed = slowSpeedRight;
    //         // } else {
    //         //     MotorAction_SetPinsToGoForward();
    //         // }

    //     } else {

    //         motorLeftSpeed = slowSpeedLeft;
    //         motorRightSpeed = slowSpeedRight - sFunction(magnitude, eeprom_max_tracking_wire_magnitude_inside, eeprom_max_tracking_wire_magnitude_outside, slowSpeedRight, onSameSiteMillis,
    //                                                      onOtherSiteMillis, eeprom_max_same_side_tracking_wire_time);
    //         if (motorRightSpeed < 0) {
    //             motorRightSpeed = 0;
    //         } else if (motorRightSpeed > slowSpeedRight) {
    //             motorRightSpeed = slowSpeedRight;
    //         }

    //         if (onSameSiteMillis > eeprom_max_same_side_tracking_wire_time) {
    //             MotorAction_SetPinsToRotateRight();
    //             motorLeftSpeed = slowSpeedLeft;
    //             motorRightSpeed = slowSpeedRight;
    //         } else {
    //             MotorAction_SetPinsToGoForward();
    //         }
    //     }

    // } else if (magnitude < 0) {

    //     if (lastInside != inside) {
    //         onSameSiteMillis = 0;
    //         lastInside = inside;
    //         onOtherSiteMillis += (currentMillisGlobal - lastTimeFollowWire);
    //     } else {
    //         onSameSiteMillis = onSameSiteMillis + (currentMillisGlobal - lastTimeFollowWire);
    //         onOtherSiteMillis = 0;
    //     }

    //     if (eeprom_perimeter_is_clockwise_from_garage == 1) {

    //         motorLeftSpeed = slowSpeedLeft;
    //         motorRightSpeed = slowSpeedRight - sFunction(magnitude, eeprom_max_tracking_wire_magnitude_inside, eeprom_max_tracking_wire_magnitude_outside, slowSpeedRight, onSameSiteMillis,
    //                                                      onOtherSiteMillis, eeprom_max_same_side_tracking_wire_time);

    //         // lcd.setCursor(0, 0);
    //         // lcd.print("RightSpeed " + String(motorRightSpeed) + "         ");
    //         // lcd.setCursor(0, 1);
    //         // lcd.print("LeftSpeed " + String(motorLeftSpeed) + "         ");

    //         if (motorRightSpeed < 0) {
    //             motorRightSpeed = 0;
    //         } else if (motorRightSpeed > slowSpeedRight) {
    //             motorRightSpeed = slowSpeedRight;
    //         }

    //         // if (onSameSiteMillis > eeprom_max_same_side_tracking_wire_time) {
    //         //     MotorAction_SetPinsToRotateRight();
    //         //     motorLeftSpeed = slowSpeedLeft;
    //         //     motorRightSpeed = slowSpeedRight;
    //         // } else {
    //         //     MotorAction_SetPinsToGoForward();
    //         // }

    //     } else {

    //         motorRightSpeed = slowSpeedRight;
    //         motorLeftSpeed = slowSpeedLeft - sFunction(magnitude, eeprom_max_tracking_wire_magnitude_inside, eeprom_max_tracking_wire_magnitude_outside, slowSpeedLeft, onSameSiteMillis,
    //                                                    onOtherSiteMillis, eeprom_max_same_side_tracking_wire_time);
    //         if (motorLeftSpeed < 0) {
    //             motorLeftSpeed = 0;
    //         } else if (motorLeftSpeed > slowSpeedLeft) {
    //             motorLeftSpeed = slowSpeedLeft;
    //         }

    // if (onSameSiteMillis > eeprom_max_same_side_tracking_wire_time) {
    //     MotorAction_SetPinsToRotateLeft();
    //     motorLeftSpeed = slowSpeedLeft;
    //     motorRightSpeed = slowSpeedRight;
    // } else {
    //     MotorAction_SetPinsToGoForward();
    // }
    //     }
    // }

    if (currentFSMSequence == FSMSEQUENCE_ZONE_1) {
        if ((millis() - startTimeFollowWire) >= eeprom_follow_wire_zone_1_time) {
            TriggerFSM(STATE_FOLLOW_WIRE, STATE_WIRE_TO_GARDEN, currentFSMSequence);
            return;
        }
    } else if (currentFSMSequence == FSMSEQUENCE_ZONE_2) {
        if ((millis() - startTimeFollowWire) >= eeprom_follow_wire_zone_2_time) {
            TriggerFSM(STATE_FOLLOW_WIRE, STATE_WIRE_TO_GARDEN, currentFSMSequence);
            return;
        }
    }

    // lastTimeFollowWire = currentMillisGlobal;
    // MotorAction_GoPWMSpeed(motorLeftSpeed, motorRightSpeed);
    read_followWire_keys();
}

void followWire_on_exit() {
    MotorAction_StopMotors();
    clearLCD();
}

State state_followWire(&followWire_on_enter, &followWire, &followWire_on_exit);
