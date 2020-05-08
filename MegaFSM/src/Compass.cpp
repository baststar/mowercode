#include <Arduino.h>
#include <Compass.h>
#include <Wire.h>

// #include <DFRobot_QMC5883.h>
// DFRobot_QMC5883 compass;

#include <QMC5883L.h>
QMC5883L compass;


// void SetupCompass() {

//     while (!compass.begin()) {
//         Serial.println("Could not find a valid QMC5883 sensor, check wiring!");
//         delay(500);
//     }

//     if (compass.isHMC()) {
//         Serial.println("Initialize HMC5883");
//         compass.setRange(HMC5883L_RANGE_1_3GA);
//         compass.setMeasurementMode(HMC5883L_CONTINOUS);
//         compass.setDataRate(HMC5883L_DATARATE_15HZ);
//         compass.setSamples(HMC5883L_SAMPLES_8);
//     } else if (compass.isQMC()) {
//         Serial.println("Initialize QMC5883");
//         compass.setRange(QMC5883_RANGE_2GA);
//         compass.setMeasurementMode(QMC5883_CONTINOUS);
//         compass.setDataRate(QMC5883_DATARATE_50HZ);
//         // compass.setSamples(QMC5883_SAMPLES_8);
//     }
// }


void SetupCompass() {
    Wire.begin();
    compass.init();
    compass.setSamplingRate(50);
}

int GetHeading() {
    int heading = compass.readHeading();
    return (int)heading;
}

// int GetHeading() {

//     Vector norm = compass.readNormalize();
//     float heading = atan2(norm.YAxis, norm.XAxis);
//     delay(250);
//     // Set declination angle on your location and fix heading
//     // You can find your declination on: http://magnetic-declination.com/
//     // (+) Positive or (-) for negative
//     // For Bytom / Poland declination angle is 4'26E (positive)
//     // For me,  +3° 37'
//     // Formula: (deg + (min / 60.0)) / (180 / PI);
//     float declinationAngle = (3.0 + (37.0 / 60.0)) / (180 / PI);
//     heading += declinationAngle;
//     // Correct for heading < 0deg and heading > 360deg
//     if (heading < 0) {
//         heading += 2 * PI;
//     }
//     if (heading > 2 * PI) {
//         heading -= 2 * PI;
//     }

//     float headingDegrees = heading * 180 / PI;

//     // // Output
//     // Serial.print(" Heading = ");
//     // Serial.print(heading);
//     // Serial.print(" Degress = ");
//     // Serial.print(headingDegrees);
//     // Serial.println();

//     return heading;
// }
