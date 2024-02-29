void taskMqttManager(void*) {
  message_t message;

  client.setServer(mqtt_server, port);
  client.setCallback(callback);

  for (;;) {
    int state = client.state();                         // send to display
    if (WiFi.status() == WL_CONNECTED && state == 0) {  // are we connected to WiFi but not to the broker?
      client.loop();                                    // successfully connected
    } else {
      if (client.connect(clientID, MQTTusername, MQTTpassword)) {
      }
    }  // check if we need to connect
    client.loop();
  }  // infinite loop
}

void callback(char* topic, byte* payload, unsigned int length) {
  message_t message;
  message.id = 0; // send message
  for (int i = 0; i < length; i++) {
    message.data[i] = (char)payload[i];
  }
  message.data[length] = '/0';
  serialQueue.sendMessage(message, 20);
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