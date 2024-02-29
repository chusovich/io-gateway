#include <freeRTOS_API.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <StreamReader.h>
// #include <menuBuilder.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
#include "secret.h"

// ----------- freeRTOS objects ----------- //
Task mqttTask("MQTT Task", 2048, 0);
Task serialInTask("Serial Read Task", 1024, 1);
Task serialOutTask("Serial Write Task", 1024, 1);
Task displayTask("Display Update Task", 2048, 1);

Queue incomingMsgs(25);
Queue outgoingMsgs(25);

void TaskMQTT(void *);
void TaskSerialRead(void *);
void TaskSerialWrite(void *);
void TaskDisplay(void *);

// ----------- setup ----------- //
void setup() {
  Serial.begin(115200);

  mqttTask.createTask(TaskMQTT,2);
  serialInTask.createTask(TaskSerialRead,1);
  serialOutTask.createTask(TaskSerialWrite,1);
  displayTask.createTask(TaskDisplay,1);

  incomingMsgs.create();
  outgoingMsgs.create();

  vTaskDelete(NULL);
}

void loop() {
// code never gets here
}