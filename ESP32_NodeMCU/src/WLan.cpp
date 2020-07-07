#include "esp_event.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include <Arduino.h>
#include <ESPmDNS.h>
#include <MQTT.h>
#include <SPIFFS.h>
#include <WLan.h>
#include <config.h>
#include <mqtt_client.h>


const char *cacert = MQTT_CA_CERT;

const char *clientcert = MQTT_CLIENT_CERT;

const char *clientkey = MQTT_CLIENT_KEY;




/* Set the SSID and Password via project configuration, or can set directly here */
#define DEFAULT_SSID WIFI_SSID
#define DEFAULT_PWD WIFI_PASS

bool wifiConnected = false;
bool mqttConnected = false;

#if CONFIG_EXAMPLE_WIFI_ALL_CHANNEL_SCAN
#define DEFAULT_SCAN_METHOD WIFI_ALL_CHANNEL_SCAN
#elif CONFIG_EXAMPLE_WIFI_FAST_SCAN
#define DEFAULT_SCAN_METHOD WIFI_FAST_SCAN
#else
#define DEFAULT_SCAN_METHOD WIFI_FAST_SCAN
#endif /*CONFIG_EXAMPLE_SCAN_METHOD*/

#if CONFIG_EXAMPLE_WIFI_CONNECT_AP_BY_SIGNAL
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SIGNAL
#elif CONFIG_EXAMPLE_WIFI_CONNECT_AP_BY_SECURITY
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SECURITY
#else
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SIGNAL
#endif /*CONFIG_EXAMPLE_SORT_METHOD*/

#if CONFIG_EXAMPLE_FAST_SCAN_THRESHOLD
#define DEFAULT_RSSI CONFIG_EXAMPLE_FAST_SCAN_MINIMUM_SIGNAL
#if CONFIG_EXAMPLE_FAST_SCAN_WEAKEST_AUTHMODE_OPEN
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#elif CONFIG_EXAMPLE_FAST_SCAN_WEAKEST_AUTHMODE_WEP
#define DEFAULT_AUTHMODE WIFI_AUTH_WEP
#elif CONFIG_EXAMPLE_FAST_SCAN_WEAKEST_AUTHMODE_WPA
#define DEFAULT_AUTHMODE WIFI_AUTH_WPA_PSK
#elif CONFIG_EXAMPLE_FAST_SCAN_WEAKEST_AUTHMODE_WPA2
#define DEFAULT_AUTHMODE WIFI_AUTH_WPA2_PSK
#else
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#endif
#else
#define DEFAULT_RSSI -127
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#endif /*CONFIG_EXAMPLE_FAST_SCAN_THRESHOLD*/

static const char *TAG = "scan";

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        char message[50];
        snprintf(message, sizeof(message), "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        Serial.println(message);
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        wifiConnected = true;
        if (!MDNS.begin("mower")) {
            // Serial.println("Error setting up MDNS responder!");
            while (1) {
                delay(1000);
            }
        }
        Serial.println("Reachable under http://mower");

        if (USE_MQTT == 1) {
            Setup_Mqtt();
        }
    }
}

/* Initialize Wi-Fi as sta and set scan method */
static void fast_scan(void) {

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, NULL));

    // Initialize default station as network interface instance (esp-netif)
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_config_t wifi_config = {};
    memset(&wifi_config, 0, sizeof(wifi_config));
    strcpy((char *)wifi_config.sta.ssid, (const char *)WIFI_SSID);
    strcpy((char *)wifi_config.sta.password, (const char *)WIFI_PASS);
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    wifi_config.sta.pmf_cfg.capable = true;
    wifi_config.sta.pmf_cfg.required = false;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void Setup_Wlan(void) {
    if (USE_WLAN == 1) {
        // Initialize NVS
        esp_err_t ret = nvs_flash_init();
        if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            ESP_ERROR_CHECK(nvs_flash_erase());
            ret = nvs_flash_init();
        }
        ESP_ERROR_CHECK(ret);
        fast_scan();
    }
}

typedef esp_mqtt_event_t *esp_mqtt_event_handle_t;
esp_mqtt_client_handle_t client;

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event) {

    Serial.println("got an mqtt event ");

    client = event->client;
    int msg_id;
    // your_context_t *context = event->context;
    switch (event->event_id) {
    case MQTT_EVENT_CONNECTED: {
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        Serial.println("MQTT_EVENT_CONNECTED");
        esp_mqtt_client_subscribe(client, "tomower", 0);
        char buf_value[512] = {0};
        char buf_topic[512] = {0};
        sprintf(buf_topic, "mower");
        sprintf(buf_value, "mower online");
        esp_mqtt_client_publish(client, buf_topic, buf_value, strlen(buf_value), 1, 1);
        mqttConnected = true;

    } break;
    case MQTT_EVENT_DISCONNECTED: {
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        Serial.println("MQTT_EVENT_DISCONNECTED");
        mqttConnected = false;
    } break;

    case MQTT_EVENT_SUBSCRIBED: {
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        Serial.println("MQTT_EVENT_SUBSCRIBED");
    } break;
    case MQTT_EVENT_UNSUBSCRIBED: {
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        Serial.println("MQTT_EVENT_UNSUBSCRIBED");
    } break;
    case MQTT_EVENT_PUBLISHED: {
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        Serial.println("MQTT_EVENT_PUBLISHED");
    } break;
    case MQTT_EVENT_DATA: {
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        Serial.println("MQTT_EVENT_DATA");
    } break;
    case MQTT_EVENT_ERROR: {
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        Serial.println("MQTT_EVENT_ERROR");
        mqttConnected = false;
    } break;
    default: {
        Serial.println("default event " + String(event->event_id));
    } break;
    }
    return ESP_OK;
}

void Setup_Mqtt() {

    if (!SPIFFS.begin(true)) {
        Serial.println("Error mounting SPIFFS");
    }

    File file = SPIFFS.open("/ca.crt", "r");
    Serial.println(file.name());

    // File file = SPIFFS.open("/ca.crt");

    if (!file) {
        Serial.println("Error getting file");
    } else {
        Serial.println("File:");
        Serial.println(file.size());
    }

    if (USE_MQTT == 1) {

        String data = file.readString();
        Serial.println(data);

        char cert[file.size() + 1];
        data.toCharArray(cert, file.size() + 1);
        // snprintf(cert, sizeof(cert), "%s", data);

        esp_mqtt_client_config_t mqtt_cfg = {};
        memset(&mqtt_cfg, 0, sizeof(mqtt_cfg));
        // mqtt_cfg.uri = MQTT_DOMAIN; // MQTT_DOMAIN;
        // mqtt_cfg.uri = "mqtt://192.168.0.20:8883"; // MQTT_DOMAIN;
        mqtt_cfg.host = MQTT_HOST; // MQTT_DOMAIN;
        mqtt_cfg.port = 1883;
        mqtt_cfg.username = MQTT_USERNAME;
        mqtt_cfg.password = MQTT_PASSWORD;
        mqtt_cfg.client_id = "esp32s2";
        mqtt_cfg.cert_pem = cert;
        mqtt_cfg.cert_len = 0;
        // mqtt_cfg.client_cert_pem = clientcert;
        // mqtt_cfg.client_key_pem = clientkey;
        mqtt_cfg.event_handle = mqtt_event_handler;

        esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
        // esp_mqtt_event_id_t mqttEventId;
        // esp_mqtt_client_register_event(client, mqttEventId, mqtt_event_handler, client);
        esp_mqtt_client_start(client);
    }
}

bool WIFIConnected() {
    return wifiConnected;
}

bool MQTTConnected() {
    return mqttConnected;
}

void SendMQTTMessage(char *topic, char *message) {

    if (!MQTTConnected()) {
        return;
    }

    char buf_value[512] = {0};
    char buf_topic[512] = {0};
    sprintf(buf_topic, topic);
    sprintf(buf_value, message);
    esp_mqtt_client_publish(client, buf_topic, buf_value, strlen(buf_value), 1, 1);
}
