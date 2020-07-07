#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <States/FSMMower.h>
#include <Wire.h>
#include <config.h>

#define LCD_COLS 16
#define LCD_ROWS 2
#define LCD_ADDR 0x27 // uint8_t

uint8_t lcd_Addr = LCD_ADDR;

const int rs = 0, rw = 1, en = 2, db4 = 4, db5 = 5, db6 = 6, db7 = 7, bl = 3;
LiquidCrystal_I2C lcd(lcd_Addr, en, rw, rs, db4, db5, db6, db7, bl, POSITIVE);

void Setup_LCD() {
    Wire.begin(PIN_SDA_DISPLAY, PIN_SCL_DISPLAY);
    lcd.begin(LCD_COLS, LCD_ROWS);
    lcd.setCursor(0, 0);
    lcd.print("Display ready");
}

void ScanI2C() {
    byte error, address; // variable for error and I2C address
    int nDevices;

    Serial.println("Scanning...");

    nDevices = 0;
    for (address = 1; address < 127; address++) {
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.print(address, HEX);
            Serial.println("  !");
            nDevices++;
        } else if (error == 4) {
            Serial.print("Unknown error at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.println(address, HEX);
        }
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");

    delay(5000); // wait 5 seconds for the next I2C scan
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

    if ((currentMillisGlobal - lastTextScroll) > scrollTextSpeed) {

        lastTextScroll = currentMillisGlobal;
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
    delay(3000);
}
