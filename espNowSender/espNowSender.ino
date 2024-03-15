#include "EspNowClient.h"
#include "freeRTOS_pp.h"

Task rxTask("Dequeue ESP-NOW", 2048, 1);
Task txTask("Send ESP-NOW", 4096, 1);
static uint8_t GATEWAY[]{ 0x36, 0x85, 0x18, 0xAB, 0xFA, 0xBC };
EspNowClient myClient(GATEWAY, "myPeerAlias");
message_t msg;
JsonDocument queueDoc;

void senderTask(void *) {
  Serial.print("Begin: ");
  Serial.println(myClient.begin());
  Serial.print("Sub: ");
  Serial.println(myClient.subscribe("myTopic"));
  for (;;) {
    myClient.publish("myTopic", "hello!");
    vTaskDelay(10000);
  }
}

void dequeueTask(void *) {
  for (;;) {
    myClient.dequeue(&msg);
    deserializeJson(queueDoc, msg.string);
    switch (queueDoc["id"].as<int>()) {
      case 1:
        break;
      default:
        Serial.println(String(queueDoc["topic"]));
        break;
    }
  }
}

void setup() {
  Serial.begin(115200);
  rxTask.create(dequeueTask);
  txTask.create(senderTask);
  vTaskDelete(NULL);
}

void loop() {
}