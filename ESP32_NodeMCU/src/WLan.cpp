#include "ArduinoJson.h"
#include <HTTPClient.h>
#include <WLan.h>
#include <WiFi.h>
#include <config.h>

const char *wanIp;

IPAddress staticIp(192, 168, 0, 230);
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192, 168, 0, 1);
IPAddress dns(8, 8, 8, 8);

// static const char *cert_sh1_fingerprint PROGMEM = CERT_SHA1_FINGERPRINT;

void Setup_Wlan() {

    if (USE_WLAN == 1) {
        WiFi.persistent(false);
        // WiFi.config(staticIp, dns, gateway, subnet);
        WiFi.mode(WIFI_STA);
        WiFi.begin(WIFI_SSID, WIFI_PASS);

        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
        }

        // Serial.print("DeviceIP: ");
        // Serial.println(WiFi.localIP());

        // HTTPClient httpClient;

        // httpClient.begin(IPIFYDOMAIN);
        // int httpCode = httpClient.GET();

        // if (httpCode > 0) {
        //     String payload = httpClient.getString();
        //     // Serial.println(httpCode);
        //     // Serial.println(payload);

        //     DynamicJsonDocument doc(1024);
        //     deserializeJson(doc, payload);
        //     wanIp = doc["ip"];

        // } else {
        //     // Serial.println("Error on HTTPRequest");
        // }

        // httpClient.end();
    }
}
