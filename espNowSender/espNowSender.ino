#include "EspNowClient.h"

Queue espNowQueue(20);
static uint8_t GATEWAY[]{ 0x36, 0x85, 0x18, 0xAB, 0xFA, 0xBC };
EspNowClient myClient(GATEWAY, "myPeerAlias");
message_t msg;
JsonDocument queueDoc;

void setup() {
  espNowQueue.create();
  myClient.setQueue(&espNowQueue);
}

void loop() {
  espNowQueue.dequeue(&msg);
  serializeJson(queueDoc, msg.string);
  switch (queueDoc["id"].as<int>()) {
    case 1:
      break;
    default:
      Serial.println(msg.string);
      break;
  }
}