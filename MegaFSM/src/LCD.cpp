#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <States/StateDockedMenu.h>
#include <States/StateParkedMenu.h>
#include <States/StateTestMenu.h>
#include <config.h>

uint8_t lcd_Addr = LCD_ADDR;
LiquidCrystal_I2C lcd(lcd_Addr, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void Setup_LCD() {
    lcd.begin(16, 2);
    lcd.clear();
}

void Show_Docked_Menu() {
    String menuName = dockedMenuNames[dockedMenu_currentMenu];
    lcd.setCursor(0, 0);
    lcd.print(menuName + "                    ");
}

void Show_Parked_Menu() {
    String menuName = parkedMenuNames[parkedMenu_currentMenu];
    lcd.setCursor(0, 0);
    lcd.print(menuName + "                    ");
}

void Show_Test_Menu() {
    String menuName = testMenuNames[testMenu_currentMenu];
    lcd.setCursor(0, 0);
    lcd.print(menuName + "                    ");
}
