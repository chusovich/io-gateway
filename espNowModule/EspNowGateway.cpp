#include "EspNowGateway.h"

extern PeerData myPeerList[NUM_PEERS];
// esp_now_register_recv_cb(reinterpret_cast<esp_now_recv_cb_t>(WifiEspNowClass::rx)) == 0 &&

void EspNowGateway::espNowCallback(const uint8_t mac[WIFIESPNOW_ALEN], const uint8_t* buf, size_t count, void* arg) {
  JsonDocument doc;
  message_t cbMsg;
  char buffer[250];
  for (int i = 0; i < static_cast<int>(count); ++i) {
    buffer[i] = static_cast<char>(buf[i]);
  }
  DeserializationError error = deserializeJson(doc, buffer, 250);
  if (!error) {
    serializeJson(doc, cbMsg.string);
    Serial.println("Json serialized!");
    //int* value = static_cast<int*>(arg);
    // Serial.printf("Value: %d\n", *value);
    Serial.println("message enquqeued!");
  }
}

void EspNowGateway::setQueue(Queue* queue) {
  _espNowQueue = queue;
}

using RxCallback = void (*)(const uint8_t mac[WIFIESPNOW_ALEN], const uint8_t* buf, size_t count, void* arg);


bool EspNowGateway::begin() {
  WiFi.persistent(false);
  WiFi.mode(WIFI_AP);
  WiFi.disconnect();
  WiFi.softAP("ESPNOW_GATEWAY", nullptr, 3);
  WiFi.softAPdisconnect(false);
  bool initBool = WifiEspNow.begin();
  if (!initBool) {
    // Serial.println("WifiEspNow.begin() failed");
    return false;
  }
  int thing = 3;
  void* queuePtr = &thing;
  WifiEspNow.onReceive(reinterpret_cast<RxCallback>(EspNowGateway::espNowCallback), nullptr);
  macAddress = WiFi.softAPmacAddress();
  return true;
}

void EspNowGateway::addPeer(uint8_t mac[6]) {
  bool alreadyAdded = false;
  // check if peer has already been added
  for (int i = 0; i <= 9; i++) {
    for (int j = 0; j <= 5; j++) {
      if (myPeerList[i].mac[j] == mac[j]) {
        alreadyAdded = true;
      } else {
        alreadyAdded = false;
        break;
      }
    }
    if (alreadyAdded) {
      break;
    }
  }
  // add the peer to an open spot
  for (int i = 0; i < NUM_PEERS; i++) {
    if (!myPeerList[i].active && !alreadyAdded) {
      WifiEspNow.addPeer(mac);

      myPeerList[i].active = true;
      myPeerList[i].mac[0] = mac[0];
      myPeerList[i].mac[1] = mac[1];
      myPeerList[i].mac[2] = mac[2];
      myPeerList[i].mac[3] = mac[3];
      myPeerList[i].mac[4] = mac[4];
      myPeerList[i].mac[5] = mac[5];
      Serial.println("Peer added!");
      Serial.print("Mac: ");
      for (int j = 0; j <= 6; j++) {
        Serial.print(myPeerList[0].mac[j]);
        Serial.print(":");
      }
      Serial.println();
      break;  // after adding the peer, stop looping
    }
  }
}

void EspNowGateway::subPeerToTopic(const uint8_t mac[6], String topic) {
  bool alreadyAdded = false;
  bool notAdded = true;
  for (int p = 0; p <= NUM_PEERS; p++) {  // for each peer
    // Serial.println("Check peer for topic...");
    if (myPeerList[p].active && notAdded) {           // if the peer is active and we haven't alrady added it
      for (int t = 0; t <= NUM_TOPICS; t++) {         // go through all of its topics
        if (myPeerList[p].topics[t].equals(topic)) {  // check if the topic has aleady been added
          alreadyAdded = true;
          Serial.println("Error: topic has already been added");
        }
      }
      if (!alreadyAdded) {  // if the topic has not been already added
        // Serial.println("Topic not already added, adding topic...");
        for (int t = 0; t <= NUM_TOPICS; t++) {  // go through all of its topics
          if (myPeerList[p].topics[t] == "") {   // and if the topic element is empty
            myPeerList[p].topics[t] = topic;     // add it to the list
            Serial.println("Topic added succesfully!");
            notAdded = false;
            break;
          }
        }
      }
    }
  }
}

void EspNowGateway::forwardMessage(String topic, String payload) {
  char buffer[250];
  JsonDocument doc;
  doc["topic"] = topic;
  doc["payload"] = payload;
  for (int i = 0; i < NUM_PEERS; i++) {  // for each peer...
    if (myPeerList[i].active) {          // if it is active...
      // Serial.print("Active Mac: "); Serial.println(peerList[i].mac[0]);
      for (int j = 0; j < NUM_TOPICS; j++) {  // search through all 12 topics
        // Serial.println("Searching Topics...");
        if (myPeerList[i].topics[j].equals(topic)) {  // if we find a match...
          // create the message buffer and send the message
          serializeJson(doc, buffer);
          WifiEspNow.send(myPeerList[i].mac, reinterpret_cast<const uint8_t*>(buffer), strlen(buffer));
          Serial.println("Message Sent!");
        }
      }
    }
  }
}

void EspNowGateway::refresh() {
  JsonDocument jsonDoc;
  jsonDoc["id"] = 2;
  for (int i = 0; i < NUM_PEERS; i++) {
    for (int j = 0; j < NUM_TOPICS; j++) {
      if (myPeerList[i].topics[j].indexOf("/") != -1) {
        jsonDoc["topic"] = myPeerList[i].topics[j];
        serializeJson(jsonDoc, Serial);
      }
    }
  }
}