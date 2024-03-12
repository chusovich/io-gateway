#ifndef EspNowClient_h
#define EspNowClient_h
#include "Arduino.h"
#include <WifiEspNow.h>

#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#endif

#include <freeRTOS_API.h>
#include <ArduinoJson.h>

void espNowCallback(const uint8_t mac[WIFIESPNOW_ALEN], const uint8_t *buf, size_t count, void *arg);

class EspNowClient {
public:
  EspNowClient(uint8_t gatewayMAC[6], const char *clientAlias);
  void setQueue(Queue *queue);
  bool begin();
  bool subscribe(char topic[]);
  bool unsubscribe(char topic[]);
  bool publish(char topic[], char payload[]);
private:
  uint8_t _gtwMac[6];
  uint8_t _myMac[6];
  char _alias[20];
  Queue *_espNowQueue;
};
#endif