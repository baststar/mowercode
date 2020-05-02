#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <config.h>

uint8_t lcd_Addr = LCD_ADDR;
LiquidCrystal_I2C lcd(lcd_Addr, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int dockedMenu_currentMenu = 0;
const int dockedMenuArraySize = 4;
String dockedMenuArray[dockedMenuArraySize] = {"Exit to zone 1", "Exit to zone 2", "Quick mowing", "Tests"};

int parkedMenu_currentMenu = 0;
const int parkedMenuArraySize = 2;
String parkedMenuArray[dockedMenuArraySize] = {"Continue mowing", "To garage"};

int testMenu_currentMenu = 0;
const int testMenuArraySize = 2;
String testMenuArray[testMenuArraySize] = {"Test Wire", "Test Relais"};

void Setup_LCD() {
    lcd.begin(16, 2);
    lcd.clear();
}

void Show_Docked_Menu() {
    String menuName = dockedMenuArray[dockedMenu_currentMenu];
    lcd.setCursor(0, 0);
    lcd.print(menuName + "                    ");
}

void Show_Parked_Menu() {
    String menuName = parkedMenuArray[parkedMenu_currentMenu];
    lcd.setCursor(0, 0);
    lcd.print(menuName + "                    ");
}

void Show_Test_Menu() {
    String menuName = testMenuArray[testMenu_currentMenu];
    lcd.setCursor(0, 0);
    lcd.print(menuName + "                    ");
}