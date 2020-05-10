#include <Arduino.h>
#include <Compass.h>
#include <Wire.h>

#include <DFRobot_QMC5883.h>
DFRobot_QMC5883 compass;

float xv, yv, zv;
float heading;
float headingDegrees;
float calibrated_values[3];

void transformation(float uncalibrated_values[3]) {
    // calibration_matrix[3][3] is the transformation matrix
    // replace M11, M12,..,M33 with your transformation matrix data
    double calibration_matrix[3][3] = {{1.103, -0.105, -0.038}, {0.038, 1.09, -0.003}, {-0.005, 0, 1.138}};
    // bias[3] is the bias
    // replace Bx, By, Bz with your bias data
    double bias[3] = {1697.201, 1001.918, 2443.618};
    // calculation
    for (int i = 0; i < 3; ++i)
        uncalibrated_values[i] = uncalibrated_values[i] - bias[i];
    float result[3] = {0, 0, 0};
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            result[i] += calibration_matrix[i][j] * uncalibrated_values[j];
    for (int i = 0; i < 3; ++i)
        calibrated_values[i] = result[i];
}

float scaler;
boolean scaler_flag = false;
float normal_vector_length;

void vector_length_stabilasation() {
    // calculate the normal vector length
    if (scaler_flag == false) {
        getHeading();
        normal_vector_length = sqrt(calibrated_values[0] * calibrated_values[0] + calibrated_values[1] * calibrated_values[1] + calibrated_values[2] * calibrated_values[2]);
        scaler_flag = true;
    }
    // calculate the current scaler
    scaler = normal_vector_length / sqrt(calibrated_values[0] * calibrated_values[0] + calibrated_values[1] * calibrated_values[1] + calibrated_values[2] * calibrated_values[2]);
    // apply the current scaler to the calibrated coordinates (global array calibrated_values)
    calibrated_values[0] = calibrated_values[0] * scaler;
    calibrated_values[1] = calibrated_values[1] * scaler;
    calibrated_values[2] = calibrated_values[2] * scaler;
}

void getHeading() {

    //-------------------QMC5883----------------//
    Vector raw = compass.readRaw();

    xv = (float)raw.XAxis;
    yv = (float)raw.YAxis;
    zv = (float)raw.ZAxis;
}

void SetupCompass() {

    while (!compass.begin()) {
        Serial.println("Could not find a valid QMC5883 sensor, check wiring!");
        delay(500);
    }

    if (compass.isHMC()) {
        Serial.println("Initialize HMC5883");
        compass.setRange(HMC5883L_RANGE_1_3GA);
        compass.setMeasurementMode(HMC5883L_CONTINOUS);
        compass.setDataRate(HMC5883L_DATARATE_15HZ);
        compass.setSamples(HMC5883L_SAMPLES_8);
    } else if (compass.isQMC()) {
        Serial.println("Initialize QMC5883");
        compass.setRange(QMC5883_RANGE_2GA);
        compass.setMeasurementMode(QMC5883_CONTINOUS);
        compass.setDataRate(QMC5883_DATARATE_50HZ);
        compass.setSamples(QMC5883_SAMPLES_8);
    }
}

long int lastCheckedCompass = 0;
int currentHeading = 0;

int GetHeadingLoop() {

    float values_from_magnetometer[3];

    getHeading(); //
    values_from_magnetometer[0] = xv;
    values_from_magnetometer[1] = yv;
    values_from_magnetometer[2] = zv;

    transformation(values_from_magnetometer);

    vector_length_stabilasation();

    heading = atan2(calibrated_values[0], -calibrated_values[1]);
    float declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / PI);
    heading += declinationAngle;

    if (heading < 0) {
        heading += 2 * PI;
    }

    if (heading > 2 * PI) {
        heading -= 2 * PI;
    }

    // Convert to degrees
    headingDegrees = heading * 180 / PI;

    return (int)headingDegrees;
}

int GetHeading() {

    long int currentMillis = millis();

    if (currentMillis - lastCheckedCompass > 500) {

        lastCheckedCompass = currentMillis;

        Vector norm = compass.readNormalize();
        float heading = atan2(norm.YAxis, norm.XAxis);
        // delay(250);
        // Set declination angle on your location and fix heading
        // You can find your declination on: http://magnetic-declination.com/
        // (+) Positive or (-) for negative
        // For Bytom / Poland declination angle is 4'26E (positive)
        // For me,  +3° 37'
        // Formula: (deg + (min / 60.0)) / (180 / PI);
        float declinationAngle = (3.0 + (37.0 / 60.0)) / (180 / PI);
        heading += declinationAngle;
        // Correct for heading < 0deg and heading > 360deg
        if (heading < 0) {
            heading += 2 * PI;
        }
        if (heading > 2 * PI) {
            heading -= 2 * PI;
        }

        currentHeading = (int)(heading * 180 / PI);
    }

    return currentHeading;
}
