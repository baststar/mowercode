#pragma once

#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

extern int dockedMenu_currentMenu;
extern const int dockedMenuArraySize;
extern int parkedMenu_currentMenu;
extern const int parkedMenuArraySize;
extern int testMenu_currentMenu;
extern const int testMenuArraySize;

void Setup_LCD();
void Show_Docked_Menu();
void Show_Parked_Menu();
void Show_Test_Menu();
