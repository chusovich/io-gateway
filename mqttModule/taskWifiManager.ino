void taskWifiManager(void *) {
  message_t displayMsg;
  displayMsg.id = 4;  // update wifi status message id
  displayMsg.string[0] = '\0';
  // Serial.println("wifi manager intialized");
  for (;;) {
    if (WiFi.status() != WL_CONNECTED) {
      strcpy(displayMsg.string, "WiFi: disconnected");
      displayQueue.enqueue(displayMsg, 1000);
      Serial.println("Wifi not connected!");
      WiFi.disconnect();
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
    } else {
      strcpy(displayMsg.string, "WiFi: connected");
      Serial.println("Wifi Connected!");
      displayQueue.enqueue(displayMsg, 1000);
    }
    vTaskDelay(15000 / portTICK_PERIOD_MS);  // this task runs every 15 sec
  }
}