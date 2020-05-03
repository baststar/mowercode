#include <Arduino.h>

int RawValueAmp;
int RawValueVolt;
int Rain_Detected;
float Amps;
float Volts;
float Volts_Last;
int Zero_Volts;

void Read_Serial1() {

    String Serial1_RX_Value = "";

    while (Serial1.available() > 0) {
        char recieved = Serial1.read();
        if (recieved != '\q' && recieved != '\j' && recieved != '\w') {
            Serial1_RX_Value = Serial1_RX_Value + (char)recieved; // hack to joining chars in correct way to Serial1_Rx_Value
        } else if (recieved == '\q') {
            RawValueAmp = Serial1_RX_Value.toInt(); // if end of value found, set AmpsTX and clear Serial1_Rx_Value temp var Serial1_Rx_Value used for holding value until \q or \j
            Serial1_RX_Value = "";
        } else if (recieved == '\j') {
            RawValueVolt = Serial1_RX_Value.toInt(); // same as upper but for VoltsTX,
            Serial1_RX_Value = "";
        } else if (recieved == '\w') {
            Rain_Detected = Serial1_RX_Value.toInt(); // same as upper but for VoltsTX
            Serial1_RX_Value = "";
        }
    }
}

void UpdateVoltAmpCharge() {

    Read_Serial1();

    int mVperAmp = 185;
    int ACSoffset = 2500;
    double VoltageAmp = 0;
    VoltageAmp = (RawValueAmp / 1024.0) * 5000; // Gets you mV
    Amps = ((VoltageAmp - ACSoffset) / mVperAmp);

    if (RawValueVolt > 100) {

        float vout = 0.0;
        float R1 = 30000;                     // Mower 330 = 3000    Mower LAM = 30000
        float R2 = 7000;                      // Mower 330 = 7000
        vout = (RawValueVolt * 5.0) / 1024.0; // see text
        Volts = vout / (R2 / (R1 + R2));
        Volts_Last = Volts;
        Zero_Volts = 0;

    } else {
        Volts = Volts_Last;
        Zero_Volts = Zero_Volts + 1;
        if (Zero_Volts > 5) {
            Volts = 0;
        }
    }
}

bool IsCharging() {
    return Amps > 0.4;
}

float GetBatteryVolt() {
    return Volts;
}
