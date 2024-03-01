void taskWifiManager(void *) {
  Serial.println("wifi manager intialized");
  for (;;) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Wifi not connected!");
      WiFi.disconnect();
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
    }
    vTaskDelay(10000 / portTICK_PERIOD_MS);  // this task runs every 10 sec
  }
}