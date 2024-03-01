#include <freeRTOS_API.h>
#include "WiFi.h"
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
#define PRO_CORE 0
#define APP_CORE 1
Task wifiManager("WiFi Manager", 4096, 1);
Task mqttManager("MQTT Connection Manager", 4096, 1);
Task mqttMessenger("MQTT Messenger", 4096, 1);
Task serialReader("Serial Read Task", 4096, 1);
//Task serialWriter("Serial Write Task", 1024, 1);
//Task displayManager("Display Update Task", 2048, 0);

Queue mqttQueue(25);
Queue serialQueue(25);

void taskWifiManager(void *);
void taskmqttManager(void *);
void taskMqttMessenger(void *);
void taskSerialReader(void *);
// void taskSerialWriter(void *);
// void taskDisplay(void *);

// ----------- setup ----------- //
void setup() {
  Serial.begin(115200);

  wifiManager.createTask(taskWifiManager,PRO_CORE);
  mqttManager.createTask(taskMqttManager,PRO_CORE);
  mqttMessenger.createTask(taskMqttMessenger,PRO_CORE);
  serialReader.createTask(taskSerialReader,APP_CORE);
  // serialWriter.createTask(taskSerialWriter,APP_CORE);
  // displayManager.createTask(taskDisplay,APP_CORE);

  mqttQueue.create();
  serialQueue.create();
  vTaskDelete(NULL);
}

void loop() {
  // code never gets here
}