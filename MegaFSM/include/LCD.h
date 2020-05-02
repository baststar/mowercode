#pragma once

#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

void Setup_LCD();
void Show_Docked_Menu();
void Show_Parked_Menu();
void Show_Test_Menu();
