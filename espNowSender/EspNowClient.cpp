#include "EspNowClient.h"

void espNowCallback(const uint8_t mac[WIFIESPNOW_ALEN], const uint8_t* buf, size_t count, void* arg) {
  JsonDocument doc;
  message_t cbMsg;
  char buffer[250];
  for (int i = 0; i < static_cast<int>(count); ++i) {
    buffer[i] = static_cast<char>(buf[i]);
  }
  DeserializationError error = deserializeJson(doc, buffer, 250);
  if (!error) {
    // for (int i = 0; i < 6; i++) {
    //   doc["mac"][i] = mac[i];
    // }
    serializeJson(doc, cbMsg.string);
    static_cast<Queue*>(arg)->enqueue(cbMsg, 1000);
  }
}

EspNowClient::EspNowClient(uint8_t gatewayMAC[6], const char* clientAlias) {
  for (int i = 0; i < 6; i++) {
    _gtwMac[i] = gatewayMAC[i];
  }
  strcpy(_alias, clientAlias);
}

void EspNowClient::setQueue(Queue* queue) {
  _espNowQueue = queue;
}

bool EspNowClient::begin() {
  WiFi.persistent(false);
  WiFi.mode(WIFI_AP);
  WiFi.disconnect();
  WiFi.softAP("ESPNOW", nullptr, 3);
  WiFi.softAPdisconnect(false);
  bool initBool = WifiEspNow.begin();
  if (!initBool) {
    // Serial.println("WifiEspNow.begin() failed");
    ESP.restart();
  }
  WifiEspNow.onReceive(espNowCallback, _espNowQueue);
  WiFi.softAPmacAddress(_myMac);
  char msgBuf[250];
  JsonDocument doc;
  for (int i = 0; i < 6; i++) {
    doc["mac"][i] = _myMac[i];
  }
  doc["alias"] = _alias;
  serializeJson(doc, msgBuf);
  return WifiEspNow.send(_gtwMac, reinterpret_cast<const uint8_t*>(msgBuf), strlen(msgBuf));
}

bool EspNowClient::subscribe(char topic[]) {
  char msgBuf[250];
  JsonDocument doc;
  doc["id"] = 4;  // subscribe msg code
  for (int i = 0; i < 6; i++) {
    doc["mac"][i] = _myMac[i];
  }
  doc["topic"] = topic;
  serializeJson(doc, msgBuf);
  return WifiEspNow.send(_gtwMac, reinterpret_cast<const uint8_t*>(msgBuf), strlen(msgBuf));
}

bool EspNowClient::unsubscribe(char topic[]) {
  char msgBuf[250];
  JsonDocument doc;
  doc["id"] = 6;  // unsubscribe msg code
  for (int i = 0; i < 6; i++) {
    doc["mac"][i] = _myMac[i];
  }
  doc["topic"] = topic;
  serializeJson(doc, msgBuf);
  return WifiEspNow.send(_gtwMac, reinterpret_cast<const uint8_t*>(msgBuf), strlen(msgBuf));
}

bool EspNowClient::publish(char topic[], char payload[]) {
  char msgBuf[250];
  JsonDocument doc;
  doc["id"] = 5;  // publish msg code
  doc["topic"] = topic;
  doc["payload"] = payload;
  serializeJson(doc, msgBuf);
  return WifiEspNow.send(_gtwMac, reinterpret_cast<const uint8_t*>(msgBuf), strlen(msgBuf));
}