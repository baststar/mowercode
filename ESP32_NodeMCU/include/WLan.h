#pragma once

#include <Arduino.h>
#include <ESPmDNS.h>
// #include <WiFiClientSecure.h>

// #include "esp_event.h"
// #include "esp_event_loop.h"
// #include "esp_log.h"
// #include "esp_system.h"
// #include "esp_wifi.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/event_groups.h"
// #include "freertos/queue.h"
// #include "freertos/semphr.h"
// #include "freertos/task.h"
// #include "lwip/dns.h"
// #include "lwip/netdb.h"
// #include "lwip/sockets.h"
// #include "nvs_flash.h"
// #include <esp_event_base.h>
// #include <mqtt_client.h>
// #include <stddef.h>
// #include <stdint.h>
// #include <stdio.h>
// #include <string.h>



// #include <MQTT.h>
// extern MQTTClient mqttClient;

// typedef esp_mqtt_event_t *esp_mqtt_event_handle_t;

void Setup_Wlan(void);
static void fast_scan(void);
void wifi_init_sta(void);
// static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
// static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event);
bool WIFIConnected();
bool MQTTConnected();
void Setup_Mqtt();
void ConnectMQTT();
void mqttconnect();
void SendMQTTMessage(char *topic, char *message);
