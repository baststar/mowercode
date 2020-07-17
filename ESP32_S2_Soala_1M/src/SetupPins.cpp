
#include <Arduino.h>
#include <SetupPins.h>
#include <config.h>
#include <driver/adc.h>
#include <driver/ledc.h>

ledc_timer_config_t ledc_timer;
ledc_channel_config_t ledc_channel_motor_1;
ledc_channel_config_t ledc_channel_motor_2;
ledc_channel_config_t ledc_channel_blades;

void Setup_All_Pins() {
    Setup_Motor_Pins();
    Setup_Membrane_Button_Pins();
    Setup_Relay_Pins();
}

void Setup_Motor_Pins() {


    ledc_timer.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_timer.duty_resolution = LEDC_TIMER_8_BIT;
    ledc_timer.timer_num = LEDC_TIMER_1;
    ledc_timer.freq_hz = 5000;
    ledc_timer.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&ledc_timer);

    ledc_channel_motor_1.channel = LEDC_CHANNEL_0;
    ledc_channel_motor_1.duty = 0;
    ledc_channel_motor_1.gpio_num = PIN_MOTOR_1_ENA;
    ledc_channel_motor_1.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_channel_motor_1.hpoint = 0;
    ledc_channel_motor_1.timer_sel = LEDC_TIMER_1;
    ledc_channel_config(&ledc_channel_motor_1);

    pinMode(PIN_MOTOR_1_IN_1, OUTPUT);
    pinMode(PIN_MOTOR_1_IN_2, OUTPUT);

    ledc_set_duty(ledc_channel_motor_1.speed_mode, ledc_channel_motor_1.channel, 0);
    ledc_update_duty(ledc_channel_motor_1.speed_mode, ledc_channel_motor_1.channel);
    digitalWrite(PIN_MOTOR_1_IN_1, LOW); // Motor 1 M Rechts
    digitalWrite(PIN_MOTOR_1_IN_2, LOW);


    ledc_channel_motor_2.channel = LEDC_CHANNEL_1;
    ledc_channel_motor_2.duty = 0;
    ledc_channel_motor_2.gpio_num = PIN_MOTOR_2_ENB;
    ledc_channel_motor_2.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_channel_motor_2.hpoint = 0;
    ledc_channel_motor_2.timer_sel = LEDC_TIMER_1;
    ledc_channel_config(&ledc_channel_motor_2);

    pinMode(PIN_MOTOR_2_IN_1, OUTPUT);
    pinMode(PIN_MOTOR_2_IN_2, OUTPUT);

    ledc_set_duty(ledc_channel_motor_2.speed_mode, ledc_channel_motor_2.channel, 0);
    ledc_update_duty(ledc_channel_motor_2.speed_mode, ledc_channel_motor_2.channel);
    digitalWrite(PIN_MOTOR_2_IN_1, LOW); // Motor 2 L Links
    digitalWrite(PIN_MOTOR_2_IN_2, LOW);


    ledc_channel_blades.channel = LEDC_CHANNEL_2;
    ledc_channel_blades.duty = 0;
    ledc_channel_blades.gpio_num = PIN_BLADES_RPWM;
    ledc_channel_blades.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_channel_blades.hpoint = 0;
    ledc_channel_blades.timer_sel = LEDC_TIMER_1;
    ledc_channel_config(&ledc_channel_blades);

    pinMode(PIN_BLADES_L_EN, OUTPUT);
    pinMode(PIN_BLADES_R_EN, OUTPUT);

    ledc_set_duty(ledc_channel_blades.speed_mode, ledc_channel_blades.channel, 0);
    ledc_update_duty(ledc_channel_blades.speed_mode, ledc_channel_blades.channel);
    digitalWrite(PIN_BLADES_L_EN, LOW); // Motor 2 L Links
    digitalWrite(PIN_BLADES_R_EN, LOW);
}

void Setup_Membrane_Button_Pins() {
    pinMode(PIN_START_KEY, INPUT_PULLUP);
    pinMode(PIN_PLUS_KEY, INPUT_PULLUP);
    pinMode(PIN_MINUS_KEY, INPUT_PULLUP);
    pinMode(PIN_STOP_KEY, INPUT_PULLUP);
}


void Setup_Relay_Pins() {
    pinMode(PIN_RELAY_MOTORS, OUTPUT); // Turn of the relay for the main battery power
    digitalWrite(PIN_RELAY_MOTORS, LOW);
}
