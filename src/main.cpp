#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <DHT20.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Arduino.h>
#include <Wire.h>
#include <WiFiClientSecure.h>
#include <Arduino_MQTT_Client.h>
#include <Server_Side_RPC.h>
#include <ThingsBoard.h>
#include <HTTPClient.h>
#include <Update.h>
#include <ArduinoJson.h>

// DHT20 Sensor
DHT20 dht20;

// Task handles
TaskHandle_t TaskHandle = NULL;

// WiFi credentials
constexpr char WIFI_SSID[] = "Minh Quan";
constexpr char WIFI_PASSWORD[] = "Minhquan0407";

// ThingsBoard settings
constexpr char TOKEN[] = "7eBiKm2upaDJBgCWgvnc";
constexpr char THINGSBOARD_SERVER[] = "app.coreiot.io";
constexpr uint16_t THINGSBOARD_PORT = 1883U;
constexpr uint16_t MAX_MESSAGE_SEND_SIZE = 256U;
constexpr uint16_t MAX_MESSAGE_RECEIVE_SIZE = 256U;
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;

// OTA settings
constexpr char OTA_VERSION_URL[] = "http://192.168.1.10:8080/version.json";
constexpr char OTA_FIRMWARE_URL[] = "http://192.168.1.10:8080/firmware.bin";
constexpr char FIRMWARE_VERSION[] = "1.0.0";
constexpr uint32_t OTA_CHECK_INTERVAL = 300000U; // 5 phút

// Telemetry interval
constexpr int16_t telemetrySendInterval = 10000U;
uint32_t previousDataSend;

// Initialize underlying client
WiFiClient espClient;
Arduino_MQTT_Client mqttClient(espClient);
Server_Side_RPC<3U, 5U> rpc;
const std::array<IAPI_Implementation *, 1U> apis = {&rpc};
ThingsBoard tb(mqttClient, MAX_MESSAGE_RECEIVE_SIZE, MAX_MESSAGE_SEND_SIZE, Default_Max_Stack_Size, apis);

// Statuses
bool subscribed = false;

// Function prototypes
void InitWiFi();
bool reconnect();
void wifiConnectTask(void *pvParameters);
void coreIotConnectTask(void *pvParameters);
void sendTelemetryTask(void *pvParameters);
void sendAttributesTask(void *pvParameters);
void tbLoopTask(void *pvParameters);
void otaUpdateTask(void *pvParameters);
void performOTAUpdate();

void setup() {
  Serial.begin(SERIAL_DEBUG_BAUD);
  delay(1000);
  InitWiFi();
  Wire.begin(GPIO_NUM_11, GPIO_NUM_12);
  dht20.begin();

  // Create tasks
  xTaskCreate(wifiConnectTask, "wifiConnectTask", 4096, NULL, 1, &TaskHandle);
  xTaskCreate(coreIotConnectTask, "coreIotConnectTask", 8192, NULL, 1, &TaskHandle);
  xTaskCreate(sendTelemetryTask, "sendTelemetryTask", 4096, NULL, 1, &TaskHandle);
  xTaskCreate(sendAttributesTask, "sendAttributesTask", 4096, NULL, 1, &TaskHandle);
  xTaskCreate(tbLoopTask, "tbLoopTask", 4096, NULL, 1, &TaskHandle);
  xTaskCreate(otaUpdateTask, "otaUpdateTask", 8192, NULL, 1, &TaskHandle);
}

void loop() {
}

void InitWiFi() {
  Serial.println("Connecting to AP ...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

bool reconnect() {
  const wl_status_t status = WiFi.status();
  if (status == WL_CONNECTED) {
    return true;
  }
  InitWiFi();
  return true;
}

void wifiConnectTask(void *pvParameters) {
  while (1) {
    if (!reconnect()) {
      vTaskDelay(pdMS_TO_TICKS(5000));
      continue;
    }
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void coreIotConnectTask(void *pvParameters) {
  while (1) {
    if (!tb.connected()) {
      Serial.printf("Connecting to: (%s) with token (%s)\n", THINGSBOARD_SERVER, TOKEN);
      if (!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) {
        Serial.println("Failed to connect");
        vTaskDelay(pdMS_TO_TICKS(5000));
        continue;
      }
      tb.sendAttributeData("macAddress", WiFi.macAddress().c_str());
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void sendTelemetryTask(void *pvParameters) {
  while (1) {
    previousDataSend = millis();
    dht20.read();

    float temperature = dht20.getTemperature();
    float humidity = dht20.getHumidity();

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT20 sensor!");
    } else {
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.print(" °C, Humidity: ");
      Serial.print(humidity);
      Serial.println(" %");

      tb.sendTelemetryData("temperature", temperature);
      tb.sendTelemetryData("humidity", humidity);
      tb.sendAttributeData("firmware_version", FIRMWARE_VERSION);
    }
    vTaskDelay(pdMS_TO_TICKS(telemetrySendInterval));
  }
}

void sendAttributesTask(void *pvParameters) {
  while (1) {
    tb.sendAttributeData("rssi", WiFi.RSSI());
    tb.sendAttributeData("channel", WiFi.channel());
    tb.sendAttributeData("bssid", WiFi.BSSIDstr().c_str());
    tb.sendAttributeData("localIp", WiFi.localIP().toString().c_str());
    tb.sendAttributeData("ssid", WiFi.SSID().c_str());
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void tbLoopTask(void *pvParameters) {
  while (1) {
    tb.loop();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void otaUpdateTask(void *pvParameters) {
  while (1) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Checking for OTA update...");
      Serial.print("Local IP: ");
      Serial.println(WiFi.localIP());
      HTTPClient http;
      Serial.print("Connecting to: ");
      Serial.println(OTA_VERSION_URL);
      http.begin(OTA_VERSION_URL);
      int httpCode = http.GET();
      Serial.print("HTTP Code: ");
      Serial.println(httpCode);

      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println("Received payload: " + payload);
        StaticJsonDocument<200> doc;
        deserializeJson(doc, payload);
        String latest_version = doc["version"];
        String firmware_url = doc["url"] | OTA_FIRMWARE_URL;

        if (latest_version != FIRMWARE_VERSION) {
          Serial.println("New firmware available: v" + latest_version);
          performOTAUpdate();
        } else {
          Serial.printf("Firmware is up to date: v%s\n", FIRMWARE_VERSION);
        }
      } else {
        Serial.printf("Failed to check OTA version, HTTP code: %d\n", httpCode);
      }
      http.end();
    } else {
      Serial.println("WiFi not connected");
    }
    vTaskDelay(pdMS_TO_TICKS(OTA_CHECK_INTERVAL));
  }
}

void performOTAUpdate() {
  tb.sendAttributeData("firmware_status", "Starting OTA update");
  HTTPClient http;
  http.begin(OTA_FIRMWARE_URL);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    int contentLength = http.getSize();
    if (contentLength <= 0) {
      Serial.println("Invalid firmware size");
      tb.sendAttributeData("firmware_status", "Invalid firmware size");
      http.end();
      return;
    }

    if (Update.begin(contentLength)) {
      Serial.println("Writing firmware...");
      size_t written = Update.writeStream(http.getStream());

      if (written == contentLength) {
        Serial.println("Firmware written successfully");
        if (Update.end(true)) {
          Serial.println("OTA update complete. Rebooting...");
          tb.sendAttributeData("firmware_status", "Update successful");
          ESP.restart();
        } else {
          Serial.println("Error finalizing update");
          tb.sendAttributeData("firmware_status", "Finalization error");
        }
      } else {
        Serial.println("Firmware write failed");
        tb.sendAttributeData("firmware_status", "Write error");
      }
    } else {
      Serial.println("Not enough space for OTA update");
      tb.sendAttributeData("firmware_status", "Insufficient space");
    }
  } else {
    Serial.printf("Failed to download firmware, HTTP code: %d\n", httpCode);
    tb.sendAttributeData("firmware_status", "Download failed");
  }
  http.end();
}