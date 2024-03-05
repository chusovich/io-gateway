void createMenus() {
  HomeScreen.addLine(0, &Title);
  HomeScreen.addLine(1, &WifiStatus);
  HomeScreen.addLine(2, &MqttStatus);
  HomeScreen.addLine(3, &RefreshButton);
  MainMenu.addScreen(0, &HomeScreen);
  MainMenu.selectScreen(0);
}

void refresh() {
  JsonDocument doc;
  doc["msgID"] = 2;  // tell the ESP-NOW module to resub to the topic in the peer list
  serializeJson(doc, Serial);
}

void timerCallback(TimerHandle_t timerHandle) {
  message_t timerMsg = { 3, "null" };
  displayQueue.enqueueISR(&timerMsg);
}
