#include <CustomFunctions.h>
#include <EEPROMVariables.h>
#include <Fsm.h>
#include <Keyboard.h>
#include <LCD.h>
#include <States/FSMMower.h>
#include <States/FSMSequences.h>
#include <States/FSMStates.h>
#include <States/Menus/StateSettingsPerimeterMenu.h>


int settingsPerimeterMenu_currentMenu = 0;
String settingsPerimeterMenuNames[] = {"CW from Grarage", "Mag inside", "Mag outside", "SameSide Error", "P-Value"};
String clockwiseString = eeprom_perimeter_is_clockwise_from_garage == 0 ? "Counterclockwise" : "Clockwise";

void read_settingsPerimeter_keys() {
    Read_Membrane_Keys();
    if (StopKey_pressed == 0) {
        delay(250);

        beforeMenuFSMTransition = currentFSMTransition;
        TriggerFSM(STATE_SETTINGS_PERIMETER_MENU, STATE_SETTINGS_MENU, -1);
        return;
    } else if (PlusKey_pressed == 0) {
        delay(100);
        if (settingsPerimeterMenu_currentMenu == 0) {
            eeprom_perimeter_is_clockwise_from_garage = (eeprom_perimeter_is_clockwise_from_garage == 0 ? 1 : 0);
            SaveIntToEEPROM(EEPROM_INDEX_PERIMETER_IS_CLOCKWISE_FROM_GARAGE, eeprom_perimeter_is_clockwise_from_garage);
        } else if (settingsPerimeterMenu_currentMenu == 1) {
            eeprom_max_tracking_wire_magnitude_inside += 10;
            SaveIntToEEPROM(EEPROM_INDEX_MAX_TRACKING_WIRE_MAGNITUDE_INSIDE, eeprom_max_tracking_wire_magnitude_inside);
        } else if (settingsPerimeterMenu_currentMenu == 2) {
            eeprom_max_tracking_wire_magnitude_outside += 10;
            SaveIntToEEPROM(EEPROM_INDEX_MAX_TRACKING_WIRE_MAGNITUDE_OUTSIDE, eeprom_max_tracking_wire_magnitude_outside);
        } else if (settingsPerimeterMenu_currentMenu == 3) {
            eeprom_max_same_side_tracking_wire_time += 100;
            SaveIntToEEPROM(EEPROM_INDEX_MAX_SAME_SIDE_TRACKING_WIRE_TIME, eeprom_max_same_side_tracking_wire_time);
        } else if (settingsPerimeterMenu_currentMenu == 4) {
            eeprom_p_value += 1;
            SaveIntToEEPROM(EEPROM_INDEX_P_VALUE, eeprom_p_value);
        }

    } else if (MinusKey_pressed == 0) {
        delay(100);
        if (settingsPerimeterMenu_currentMenu == 0) {
            eeprom_perimeter_is_clockwise_from_garage = (eeprom_perimeter_is_clockwise_from_garage == 0 ? 1 : 0);
            SaveIntToEEPROM(EEPROM_INDEX_PERIMETER_IS_CLOCKWISE_FROM_GARAGE, eeprom_perimeter_is_clockwise_from_garage);
        } else if (settingsPerimeterMenu_currentMenu == 1) {
            eeprom_max_tracking_wire_magnitude_inside -= 10;
            SaveIntToEEPROM(EEPROM_INDEX_MAX_TRACKING_WIRE_MAGNITUDE_INSIDE, eeprom_max_tracking_wire_magnitude_inside);
        } else if (settingsPerimeterMenu_currentMenu == 2) {
            eeprom_max_tracking_wire_magnitude_outside -= 10;
            SaveIntToEEPROM(EEPROM_INDEX_MAX_TRACKING_WIRE_MAGNITUDE_OUTSIDE, eeprom_max_tracking_wire_magnitude_outside);
        } else if (settingsPerimeterMenu_currentMenu == 3) {
            eeprom_max_same_side_tracking_wire_time -= 100;
            SaveIntToEEPROM(EEPROM_INDEX_MAX_SAME_SIDE_TRACKING_WIRE_TIME, eeprom_max_same_side_tracking_wire_time);
        } else if (settingsPerimeterMenu_currentMenu == 4) {
            eeprom_p_value -= 1;
            SaveIntToEEPROM(EEPROM_INDEX_P_VALUE, eeprom_p_value);
        }

    } else if (StartKey_pressed == 0) {
        delay(250);
        settingsPerimeterMenu_currentMenu++;
        if (settingsPerimeterMenu_currentMenu >= ARRAY_SIZE(settingsPerimeterMenuNames)) {
            settingsPerimeterMenu_currentMenu = 0;
        }
    }
}

void settingsPerimeter_on_enter() {
    settingsPerimeterMenu_currentMenu = 0;
}
void settingsPerimeter() {
    String menuname = settingsPerimeterMenuNames[settingsPerimeterMenu_currentMenu];
    lcd.setCursor(0, 0);
    lcd.print(menuname + "           ");

    lcd.setCursor(0, 1);
    lcd.write(126);
    if (settingsPerimeterMenu_currentMenu == 0) {
        clockwiseString = eeprom_perimeter_is_clockwise_from_garage == 0 ? "Counterclockwise" : "Clockwise";
        lcd.print(" " + clockwiseString + "            ");
    } else if (settingsPerimeterMenu_currentMenu == 1) {
        lcd.print(" " + String(eeprom_max_tracking_wire_magnitude_inside) + "            ");
    } else if (settingsPerimeterMenu_currentMenu == 2) {
        lcd.print(" " + String(eeprom_max_tracking_wire_magnitude_outside) + "            ");
    } else if (settingsPerimeterMenu_currentMenu == 3) {
        lcd.print(" " + String(eeprom_max_same_side_tracking_wire_time) + " ms           ");
    } else if (settingsPerimeterMenu_currentMenu == 4) {
        lcd.print(" " + String(eeprom_p_value / 100.0) + "              ");
    }

    read_settingsPerimeter_keys();
}
void settingsPerimeter_on_exit() {
    clearLCD();
}
State state_settingsPerimeterMenu(&settingsPerimeter_on_enter, &settingsPerimeter, &settingsPerimeter_on_exit);
