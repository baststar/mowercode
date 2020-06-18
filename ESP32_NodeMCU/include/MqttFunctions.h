#pragma once
#include <MQTT.h>

extern MQTTClient mqttClient;

void Setup_Mqtt();
void ConnectMQTT();
void receivedCallback(char *topic, byte *payload, unsigned int length);
void mqttconnect();
