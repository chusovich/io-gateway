void taskMqttManager(void*) {
  // Serial.println("mqtt manager intialized");
  message_t message;
  message_t displayMsg;
  displayMsg.id = 5;
  displayMsg.string[0] = '\0';
  client.setServer(mqtt_server, port);
  client.setCallback(callback);

  for (;;) {
    // if we're connected, call the loop function and wait for 100 ms, other try to connect and then wait for 2s
    if (WiFi.status() == WL_CONNECTED) {
      if (client.connected()) {
        client.loop();
        vTaskDelay(200 / portTICK_PERIOD_MS);
      } else {
        // Serial.println("attempting to connect");
        if (client.connect(clientID, MQTTusername, MQTTpassword)) {
          // send json message over serial to ESP-NOW module
          JsonDocument doc;
          doc["msgID"] = 2;  // cmd to the topic in the peer list
          serializeJson(doc, Serial1);
          // updat display
          strcpy(displayMsg.string, "MQTT: connected"); Serial.println("MQTT Connected");
          displayQueue.enqueue(displayMsg, 1000);
        } else {
          strcpy(displayMsg.string, "MQTT: disconnected"); Serial.println("MQTT Disconnected");
          displayQueue.enqueue(displayMsg, 1000);
          delay(100);
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
      }
    } else {
      vTaskDelay(1000);
    }
  }  // infinite loop
}  // task function

void callback(char* topic, byte* payload, unsigned int length) {
  JsonDocument doc;
  char data[32];
  for (int i = 0; i < length; i++) {
    data[i] = (char)payload[i];
  }
  data[length] = '\0';
  doc["id"] = 1;
  doc["payload"] = data;
  doc["topic"] = topic;
  serializeJson(doc, Serial1);
  Serial.println("mqtt cb");
}