#pragma once

#include <Arduino.h>
// #include <DFRobot_QMC5883.h>
// extern DFRobot_QMC5883 compass;

#include <QMC5883L.h>
extern QMC5883L compass;

void SetupCompass();
int GetHeading();
