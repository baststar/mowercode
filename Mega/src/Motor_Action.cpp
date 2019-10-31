#include <Arduino.h>
#include <config.h>
#include <declarations.h>

void Motor_Action_Go_Full_Speed()
{
    analogWrite(PIN_ENA, PWM_MaxSpeed_RH); // Speed = 0-255  (255 is max speed). Speed is set in the settings
    analogWrite(PIN_ENB, PWM_MaxSpeed_LH); // Anaolgwirte sends PWM signals Speed = 0-255  (255 is max speed)
    Serial.print(F("Wheel:FULL|"));
}

void Motor_Action_GoFullSpeed_Out_Garage()
{
    // Speeds can be changed to give the mower a slight curve when exiting the Garage.

    PWM_MaxSpeed_LH = PWM_MaxSpeed_LH + 20;
    if (PWM_MaxSpeed_LH > 255)
        PWM_MaxSpeed_LH = 255;
    if (PWM_MaxSpeed_RH > 255)
        PWM_MaxSpeed_RH = 255;

    analogWrite(PIN_ENA, PWM_MaxSpeed_RH); // Speed = 0-255  (255 is max speed). Speed is set in the settings
    analogWrite(PIN_ENB, PWM_MaxSpeed_LH);
    Serial.print(F("Wheel:FULL|"));
}

void SetPins_ToGoForwards()
{                                // Motor Bridge pins are set for both motors to move forwards.
    digitalWrite(PIN_IN_1, LOW); // Motor Birdge pins are set to high or low to set the direction of movement
    digitalWrite(PIN_IN_2, HIGH);
    digitalWrite(PIN_IN_3, LOW);
    digitalWrite(PIN_IN_4, HIGH);
    Serial.print(F("Wheel:For|"));
}

void SetPins_ToGoBackwards()
{                                 // Motor Bridge pins are set for both motors to move Backwards
    digitalWrite(PIN_IN_1, HIGH); // Motor 1
    digitalWrite(PIN_IN_2, LOW);
    digitalWrite(PIN_IN_3, HIGH); // Motor 2
    digitalWrite(PIN_IN_4, LOW);
    Serial.print(F("Wheel:Rev|"));
    delay(20);
}

void Motor_Action_Stop_Motors()
{ // Motor Bridge pins are set for both motors to stop
    digitalWrite(PIN_ENA, 0);
    digitalWrite(PIN_IN_1, LOW); // Motor 1
    digitalWrite(PIN_IN_2, LOW);

    digitalWrite(PIN_ENB, 0); // Motor 2
    digitalWrite(PIN_IN_3, LOW);
    digitalWrite(PIN_IN_4, LOW);

    Serial.print(F("Wheel:0FF|"));
}

void SetPins_ToTurnLeft()
{                                // Pins are set so that Motors drive in opposite directions
    digitalWrite(PIN_IN_1, LOW); // Motor 1
    digitalWrite(PIN_IN_2, HIGH);
    digitalWrite(PIN_IN_3, HIGH); // Motor 2
    digitalWrite(PIN_IN_4, LOW);
    Serial.print(F("Wheel:TL_|"));
}

void SetPins_ToTurnRight()
{                                 // Pins are set so that Motors drive in opposite directions
    digitalWrite(PIN_IN_1, HIGH); // Motor 1
    digitalWrite(PIN_IN_2, LOW);
    digitalWrite(PIN_IN_3, LOW); // Motor 2
    digitalWrite(PIN_IN_4, HIGH);
    Serial.print(F("Wheel:R|"));
}

// USed to turn the mower at a set speed.
void Motor_Action_Turn_Speed()
{
    analogWrite(PIN_ENA, (PWM_MaxSpeed_RH - Turn_Adjust)); // Change the 0 value to 10 or 20 to recuce the speed
    analogWrite(PIN_ENB, (PWM_MaxSpeed_LH - Turn_Adjust)); // Change the 0 value to 10 or 20 to recuce the speed
}

// Turns the mowing blades on
void Motor_Action_Spin_Blades()
{
    if (Cutting_Blades_Activate == 1) { // Blades are turn ON in settings and will spin!
        delay(20);
        digitalWrite(PIN_R_EN, HIGH);
        digitalWrite(PIN_L_EN, HIGH);
        delay(20);
        analogWrite(PIN_RPWM, PWM_Blade_Speed);
        delay(20);
        Serial.print(F("Blades:ON_|"));
    }

    if (Cutting_Blades_Activate == 0) { // Blades are turn off in settings and will not spin!
        void StopSpinBlades();
    }
}

void Motor_Action_Stop_Spin_Blades()
{
    delay(20);
    digitalWrite(PIN_R_EN, LOW);
    digitalWrite(PIN_L_EN, LOW);
    delay(20);
    Serial.print(F("Blades:0FF|"));
}

// Steers the Mower depending on the PID input from the Algorythm
void Motor_Action_Dynamic_PWM_Steering()
{
    analogWrite(PIN_ENA, PWM_Right); // ENA low = Right Swerve   ENB low = Left Swerve
    analogWrite(PIN_ENB, PWM_Left);
    Serial.print(F("PWM_R:"));
    Serial.print(PWM_Right);
    Serial.print(F("|"));
    Serial.print(F("PWM_L:"));
    Serial.print(PWM_Left);
    Serial.print(F("|"));
}
