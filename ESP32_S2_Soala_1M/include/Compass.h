#pragma once

#include <Arduino.h>
#include <DFRobot_QMC5883.h>
extern DFRobot_QMC5883 compass;

void SetupCompass();
int GetHeading();

void getHeading();
int GetHeadingLoop();
void vector_length_stabilasation();
void transformation(float uncalibrated_values[3]);
