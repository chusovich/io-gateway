void TaskMQTT(void*) {
  message_t message;
  WiFiClient espClient;
  PubSubClient client(espClient);
  client.setServer(mqtt_server, port);
  client.setCallback(callback);

  for (;;) {
    wifiReconnect();
    if (!client.connected()) {
      if (client.connect(clientID, MQTTusername, MQTTpassword)) {
        // successfully connected
      } else {
        vTaskDelay(2000 / portTICK_PERIOD_MS);
      }
      client.state();  // update display
    }
    if (incomingMsgs.receiveMessage(&message, 100)) {  // check if we have a message waiting for us
      if (message.name.equals("pub")) {
        int msgIndex = message.data.indexOf(",");
        int length = message.data.length();
        char topic[msgIndex + 1];
        message.data.substring(0, msgIndex + 1).toCharArray(topic, msgIndex + 1);
        char payload[length - msgIndex];
        message.data.substring(msgIndex + 1, length).toCharArray(payload, length - msgIndex);
        if (msgIndex == -1) {
          // failed, do nothing, don't publish or dequeue message
        } else {
          if (client.publish(topic, payload)) {
            // dequeue the message
          }
        }
      }
      if (message.name.equals("sub")) {
        int strLength = message.data.length() + 1;
        char topic[strLength];
        message.data.toCharArray(topic, strLength);
        if (client.subscribe(topic)) {
          // dequeue the message
        }
      }
      if (message.name.equals("unsub")) {
        int strLength = message.data.length() + 1;
        char topic[strLength];
        message.data.toCharArray(topic, strLength);
        if (client.unsubscribe(topic)) {
          // dequeue the message
        }
      }
    } else {
      client.loop();  // if we didn't do anything, just ping the broker,
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print(">sendMsg:");
  Serial.print(topic);
  Serial.print(",");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.print("!");
}

void wifiReconnect() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    int i = 0;
    while (i < 50) {  // try to connected for 5 seconds
      vTaskDelay(100 / portTICK_PERIOD_MS);
      if (WiFi.status() == WL_CONNECTED) {
        WiFi.localIP();  // update display
        break;
      } else {
        i++;
      }
    }
  }
}