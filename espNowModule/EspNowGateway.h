#ifndef EspNowGateway_h
#define EspNowGateway_h
#include "Arduino.h"
#include "PeerData.h"
#include <freeRTOS_API.h>
#include <WifiEspNow.h>
#include <ArduinoJson.h>

void espNowCallback(const uint8_t mac[WIFIESPNOW_ALEN], const uint8_t *buf, size_t count, void *arg);

#define NUM_PEERS 10

PeerData myPeerList[NUM_PEERS];

class EspNowGateway {
public:
  // functions
  void setQueue(Queue *queue);
  void begin();
  void addPeer(uint8_t mac[6]);
  void refresh();
  void subPeerToTopic(const uint8_t mac[6], String topic);
  // removePeer();
  void forwardMessageToPeers(String topic, String payload);
  // data
  String macAddress;
private:
  Queue *_espNowQueue;
  // for array of type: type arr_name[size];
  // type *ptr_name = &arr_name
};
#endif