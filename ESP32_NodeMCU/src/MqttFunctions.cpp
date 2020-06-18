#include <Arduino.h>
#include <CustomFunctions.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <MQTT.h>
#include <MqttFunctions.h>
#include <WLan.h>
#include <WiFiClientSecure.h>
#include <config.h>

MQTTClient mqttClient;
WiFiClientSecure espClient;

void Setup_Mqtt() {

    if (!MDNS.begin("esp32")) {
        // Serial.println("Error setting up MDNS responder!");
        while (1) {
            delay(1000);
        }
    }

    if (USE_MQTT == 1 && USE_WLAN == 1) {
        mqttClient.begin(MQTT_HOST, MQTT_PORT, espClient);
        if (!mqttClient.connected()) {
            ConnectMQTT();
        }
    }
}

void ConnectMQTT() {
    // Serial.println("Attempting MQTT connection...");
    while (!mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
        // Serial.print("Attempting MQTT connection...");
        delay(5000);
    }
    // mqttClient.publish("/SmartHome/door/open", "1");
    mqttClient.publish("mower", "mower is online!");
}
