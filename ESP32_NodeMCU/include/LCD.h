#pragma once

#include <LiquidCrystal_I2C.h>

using namespace std;

extern LiquidCrystal_I2C lcd;

void Setup_LCD();
void Show_Docked_Menu();
void Show_Parked_Menu();
void Show_Test_Menu();
void Show_Settings_Menu();
void Show_SettingsMotorspeeds_Menu();
void Show_SettingsPerimeter_Menu();
void ResetScrollRow0Text();
void ScrollRow0Text(String row0text, String row0Text);
void clearLCD();
void ShowError(String errormessage);
void scan1();
