void taskWifiManager(void *) {
  for (;;) {
    if (WiFi.status() != WL_CONNECTED) {
      WiFi.disconnect();
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
    }
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}