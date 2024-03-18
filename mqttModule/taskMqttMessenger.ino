void taskMqttMessenger(void*) {
  message_t msg;
  JsonDocument doc;
  DeserializationError jsonError;
  char topic[50];
  char payload[32];

  for (;;) {
    mqttQueue.peek(&msg);  // see if we have a message
    jsonError = deserializeJson(doc, msg.string);
    // Serial.printf("message received: %s\n", msg.string);
    if (jsonError) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(jsonError.c_str());
      mqttQueue.dequeue(&msg);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      switch (doc["id"].as<int>()) {
        case 5:  // pub
          strlcpy(topic, doc["topic"] | "<null>", 32);
          strlcpy(payload, doc["payload"] | "<null>", 32);
          if (strcmp(topic, "<null>") != 0) {
            if (client.publish(topic, payload)) {
              // Serial.printf("topic to pub: %s\n", topic);
              // Serial.printf("payload to pub: %s\n", payload);
              mqttQueue.dequeue(&msg, 100);
              topic[0] = '\0';
              payload[0] = '\0';
              // Serial.println("success");
            }
          }
          break;
        case 4:  // sub
          strlcpy(topic, doc["topic"] | "<null>", 32);
          if (strcmp(topic, "<null>") != 0) {
            if (client.subscribe(topic)) {
              Serial.printf("topic to pub: %s\n", topic);
              mqttQueue.dequeue(&msg, 100);
              topic[0] = '\0';
            }
          }
          break;
        case 6:  // unsub
          Serial.println("Unsub");
          strlcpy(topic, doc["topic"] | "<null>", 32);
          if (strcmp(topic, "<null>") != 0) {
            if (client.unsubscribe(doc["topic"])) {
              mqttQueue.dequeue(&msg, 100);
              topic[0] = '\0';
            }
          }
          break;
      }  // switch statement
    }    // else
  }      // infinite loop
}