#include <freeRTOS_API.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <StreamReader.h>
// #include <menuBuilder.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
#include "secret.h"
// ----------- MQTT objects ----------- //
WiFiClient espClient;
PubSubClient client(espClient);

// ----------- freeRTOS objects ----------- //
#define pro_core 0
#define app_core 1
Task wifiManager("Wifi Connection Manager", 1024, 1);
Task mqttManager("MQTT Connection Manager", 1024, 1);
Task mqttMessenger("MQTT Messenger", 2048, 1);
Task serialReader("Serial Read Task", 1024, 2);
Task serialWriter("Serial Write Task", 1024, 2);
Task displayManager("Display Update Task", 2048, 1);

Queue mqttQueue(25);
Queue serialQueue(25);

void taskMqttManager(void *);
void taskWifiManager(void *);
void taskMqttMessenger(void *);
void taskSerialRead(void *);
void taskSerialWrite(void *);
void taskDisplay(void *);

StreamReader reader(&Serial, 2);

// ----------- setup ----------- //
void setup() {
  Serial.begin(115200);

  mqttManager.createTask(taskMqttManager, pro_core);
  mqttMessenger.createTask(taskMqttMessenger, pro_core);
  serialReader.createTask(taskSerialRead, app_core);
  serialWriter.createTask(taskSerialWrite, app_core);
  displayManager.createTask(taskDisplay, app_core);

  mqttQueue.create();
  serialQueue.create();

  vTaskDelete(NULL);
}

void loop() {
  // code never gets here
}