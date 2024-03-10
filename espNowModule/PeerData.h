#ifndef PeerData_h
#define PeerData_h
#include "Arduino.h"

#define NUM_TOPICS 12

class PeerData {
  public:
  bool active = false;
  uint8_t mac[6] = { 0, 0, 0, 0, 0, 0 };
  String topics[NUM_TOPICS];
  String alias;
};
#endif