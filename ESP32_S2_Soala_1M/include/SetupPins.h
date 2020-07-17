#pragma once

#include <driver/ledc.h>

extern ledc_channel_config_t ledc_channel_motor_1;
extern ledc_channel_config_t ledc_channel_motor_2;
extern ledc_channel_config_t ledc_channel_blades;

void Setup_All_Pins();
void Setup_Motor_Pins();
void Setup_Membrane_Button_Pins();
void Setup_Bumper_Bar_Pins();
void Setup_Relay_Pins();
void Setup_VoltAmpRain_Pins();
