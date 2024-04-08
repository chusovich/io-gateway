#include "espNowGateway.h"
#include "freeRTOS_pp.h"

Task rxTask("Get ESP-NOW", 4096, 1);
// Task txTask("Send ESP-NOW", 4096, 1);
static uint8_t GATEWAY[]{ 0x36, 0x85, 0x18, 0xAB, 0xFA, 0xBC };
EspNowClient myClient(GATEWAY, "myPeerAlias");
message_t msg;
JsonDocument queueDoc;

// void senderTask(void *) {
//   vTaskDelay(5000 / portTICK_PERIOD_MS);
//   Serial.println(myClient.subscribe("Switches/BedroomLamp/Value"));
//   vTaskDelay(1000 / portTICK_PERIOD_MS);
//   for (;;) {
//     myClient.publish("Switches/BedroomLamp/Value", "0");
//     vTaskDelay(2000 / portTICK_PERIOD_MS);
//     myClient.publish("Switches/BedroomLamp/Value", "1");
//     vTaskDelay(2000 / portTICK_PERIOD_MS);
//   }
// }

void dequeueTask(void *) {
  Serial.println(myClient.subscribe("LEDs/Kitchen1/Mode"));
  for (;;) {
    myClient.dequeue(&msg);
    deserializeJson(queueDoc, msg.string);
    // Serial.println("Message dequeued!");
    Serial.printf("message string: %s\n", msg.string);
    switch (queueDoc["payload"].as<int>()) {
      case 0:
        // digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("Case 0");
        break;
      case 1:
        // digitalWrite(LED_BUILTIN, LOW);
        Serial.println("Case 1");
        break;
    }
  }
}

void setup() {
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  Serial.begin(115200);
  //npinMode(LED_BUILTIN, OUTPUT);
  myClient.begin();
  rxTask.create(dequeueTask);
  // txTask.create(senderTask);
  // vTaskDelete(NULL);
}

void loop() {
}