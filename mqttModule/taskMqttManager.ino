void taskMqttManager(void*) {
  message_t message;
  client.setServer(mqtt_server, port);
  client.setCallback(callback);

  for (;;) {
    // if we're connected, call the loop function and wait for 100 ms, other try to connect and then wait for 2s
    if (client.connected() && WiFi.status() == WL_CONNECTED) {
      client.loop();
      vTaskDelay(200 / portTICK_PERIOD_MS);
    } else {
      client.connect(clientID, MQTTusername, MQTTpassword);
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
  }  // infinite loop
}  // task function

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("0");
  Serial.print(",");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.print("!");
}