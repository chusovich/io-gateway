void taskWifiManager(void *) {
  int wifiStatus = WiFi.status(); // update globabal variable for the display
  if (wifiStatus != 1) {
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
  }
  vTaskDelay(10000/ portTICK_PERIOD_MS); // this task runs every 10 sec
}