#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <config.h>

#define ARRAY_SIZE(x) sizeof(x) / sizeof(x[0])

#define LCD_COLS 16
#define LCD_ROWS 2
#define LCD_ADDR 0x27 // uint8_t

uint8_t lcd_Addr = LCD_ADDR;

const int rs = 0, rw = 1, en = 2, db4 = 4, db5 = 5, db6 = 6, db7 = 7, bl = 3;
LiquidCrystal_I2C lcd(lcd_Addr, en, rw, rs, db4, db5, db6, db7, bl, POSITIVE);

void Setup_LCD() {
    lcd.begin(LCD_COLS, LCD_ROWS);
    clearLCD();
}

int scrollTextPosition = 0;
bool scrollTextLeft = true;
int scrollTextSpeed = 250;
int pauseAtStartTime = 1000;
int currentPauseTime = pauseAtStartTime;
int lastTextScroll = 0;

void clearLCD() {
    lcd.clear();
}

void ResetScrollRow0Text() {
    scrollTextPosition = 0;
    scrollTextLeft = true;
    currentPauseTime = pauseAtStartTime;
    lastTextScroll = 0;
}

void ScrollRow0Text(String row0text, String row1Text) {

    lcd.setCursor(0, 0);
    lcd.print(row0text);

    int currentMillies = millis();
    if ((currentMillies - lastTextScroll) > scrollTextSpeed) {

        lastTextScroll = currentMillies;
        currentPauseTime -= scrollTextSpeed;

        if (currentPauseTime <= 0) {
            if (scrollTextLeft) {
                scrollTextPosition++;
                if (scrollTextPosition <= (row0text.length() - LCD_COLS)) {
                    lcd.scrollDisplayLeft();
                    // reset row1text
                    lcd.setCursor(scrollTextPosition, 1);
                    lcd.print(row1Text + "                  ");
                } else {
                    scrollTextLeft = false;
                    scrollTextPosition = 0;
                    currentPauseTime = pauseAtStartTime;
                }
            } else {
                scrollTextPosition++;
                if (scrollTextPosition <= (row0text.length() - LCD_COLS)) {
                    lcd.scrollDisplayRight();
                    // reset row1text
                    int initialCursor = row0text.length() - LCD_COLS;
                    lcd.setCursor(initialCursor, 1);
                    for (int i = 0; i < scrollTextPosition; i++) {
                        lcd.moveCursorLeft();
                    }
                    lcd.print(row1Text + "                  ");
                } else {
                    scrollTextLeft = true;
                    scrollTextPosition = 0;
                    currentPauseTime = pauseAtStartTime;
                }
            }
        } else {
            if (scrollTextLeft) {
                lcd.setCursor(0, 1);
                lcd.print(row1Text + "                  ");
            } else {
                int initialCursor = row0text.length() - LCD_COLS;
                lcd.setCursor(initialCursor, 1);
                lcd.print(row1Text + "                  ");
            }
        }
    }
}

void ShowError(String errormessage) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(errormessage + "                ");
    delay(2000);
}
