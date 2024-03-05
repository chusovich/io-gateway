#ifndef EspNowGateway_h
#define EspNowGateway_h
#include "Arduino.h"
#include "freeRTOS_API.h"
#include <WifiEspNow.h>
#include <ArduinoJson.h>

#define NUM_PEERS 10
#define NUM_TOPICS 12

void espNowCallback(const uint8_t mac[WIFIESPNOW_ALEN], const uint8_t* buf, size_t count, void* arg);

class EspNowGateway {
public:
  // functions
  void setQueue(Queue* queue);
  void begin();
  void addPeer(uint8_t mac[6]);
  void refresh();
  void subPeerToTopic(const uint8_t mac[6], String topic);
  // removePeer();
  void forwardMessageToPeers(String topic, String payload);
  // data
  String macAddress;
  Queue* _espNowQueue;
private:
};
#endif