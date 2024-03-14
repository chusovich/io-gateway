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
    return false;
  }
  initBool = WifiEspNow.addPeer(_gtwMac);
  if (!initBool) {
    return false;
  }
  WifiEspNow.onReceive(espNowCallback, _espNowQueue);
  WiFi.softAPmacAddress(_myMac);
  char msgBuf[245];
  JsonDocument doc;
  for (int i = 0; i < 6; i++) {
    doc["mac"][i] = _myMac[i];
  }
  doc["alias"] = _alias;
  serializeJson(doc, msgBuf);
  Serial.printf("Sending msg: %s\n", msgBuf);
  Serial.printf("Gatway MAC:%02X:%02X:%02X:%02X:%02X:%02X\n", _gtwMac[0], _gtwMac[1], _gtwMac[2], _gtwMac[3], _gtwMac[4], _gtwMac[5]);
  if (WifiEspNow.send(_gtwMac, reinterpret_cast<const uint8_t*>(msgBuf), strlen(msgBuf))) {
    return true;
  }
  return false;
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
  Serial.printf("Sending msg: %s\n", msgBuf);
  if (WifiEspNow.send(_gtwMac, reinterpret_cast<const uint8_t*>(msgBuf), strlen(msgBuf))) {
    return true;
  }
  return false;
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
  Serial.printf("Sending msg: %s\n", msgBuf);
  if (WifiEspNow.send(_gtwMac, reinterpret_cast<const uint8_t*>(msgBuf), strlen(msgBuf))) {
    return true;
  }
  return false;
}

bool EspNowClient::publish(char topic[], char payload[]) {
  char msgBuf[250];
  JsonDocument doc;
  doc["id"] = 5;  // publish msg code
  doc["topic"] = topic;
  doc["payload"] = payload;
  serializeJson(doc, msgBuf);
  Serial.printf("Sending msg: %s\n", msgBuf);
  if (WifiEspNow.send(_gtwMac, reinterpret_cast<const uint8_t*>(msgBuf), strlen(msgBuf))) {
    return true;
  }
  return false;
}