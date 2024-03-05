#include "EspNowGateway.h"

void espNowCallback(const uint8_t mac[WIFIESPNOW_ALEN], const uint8_t* buf, size_t count, void* arg) {
  JsonDocument doc;
  message_t cbMsg;
  char buffer[250];
  for (int i = 0; i < static_cast<int>(count); ++i) {
    buffer[i] = static_cast<char>(buf[i]);
  }
  DeserializationError error = deserializeJson(doc, buffer, 250);
  if (!error) {
    for (int i = 0; i < 6; i++) {
      doc["mac"][i] = mac[i];
    }
    serializeJson(doc, cbMsg.string);
    static_cast<Queue *>(arg)->enqueue(cbMsg, 1000);
  }
}

void EspNowGateway::setQueue(Queue *queue) {
  _espNowQueue = queue;
}

void EspNowGateway::begin() {
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
  macAddress = WiFi.softAPmacAddress();
}

void EspNowGateway::addPeer(uint8_t mac[6]) {
  bool alreadyAdded = false;
  // check if peer has already been added
  for (int i = 0; i <= 9; i++) {
    for (int j = 0; j <= 5; j++) {
      if (peerList[i].mac[j] == mac[j]) {
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
    if (!peerList[i].active && !alreadyAdded) {
      WifiEspNow.addPeer(mac);

      peerList[i].active = true;
      peerList[i].mac[0] = mac[0];
      peerList[i].mac[1] = mac[1];
      peerList[i].mac[2] = mac[2];
      peerList[i].mac[3] = mac[3];
      peerList[i].mac[4] = mac[4];
      peerList[i].mac[5] = mac[5];
      Serial.println("Peer added!");
      Serial.print("Mac: ");
      for (int j = 0; j <= 6; j++) {
        Serial.print(peerList[0].mac[j]);
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
    if (peerList[p].active && notAdded) {           // if the peer is active and we haven't alrady added it
      for (int t = 0; t <= NUM_TOPICS; t++) {       // go through all of its topics
        if (peerList[p].topics[t].equals(topic)) {  // check if the topic has aleady been added
          alreadyAdded = true;
          Serial.println("Error: topic has already been added");
        }
      }
      if (!alreadyAdded) {  // if the topic has not been already added
        // Serial.println("Topic not already added, adding topic...");
        for (int t = 0; t <= NUM_TOPICS; t++) {  // go through all of its topics
          if (peerList[p].topics[t] == "") {     // and if the topic element is empty
            peerList[p].topics[t] = topic;       // add it to the list
            Serial.println("Topic added succesfully!");
            notAdded = false;
            break;
          }
        }
      }
    }
  }
}

void EspNowGateway::forwardMessageToPeers(String topic, String payload) {
  char buffer[250];
  JsonDocument doc;
  doc["topic"] = topic;
  doc["payload"] = payload;
  for (int i = 0; i < NUM_PEERS; i++) {  // for each peer...
    if (peerList[i].active) {            // if it is active...
      // Serial.print("Active Mac: "); Serial.println(peerList[i].mac[0]);
      for (int j = 0; j < NUM_TOPICS; j++) {  // search through all 12 topics
        // Serial.println("Searching Topics...");
        if (peerList[i].topics[j].equals(topic)) {  // if we find a match...
          // create the message buffer and send the message
          serializeJson(doc, buffer);
          WifiEspNow.send(peerList[i].mac, reinterpret_cast<const uint8_t*>(buffer), strlen(buffer));
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
      if (peerList[i].topics[j].indexOf("/") != -1) {
        jsonDoc["topic"] = peerList[i].topics[j];
        serializeJson(jsonDoc, Serial);
      }
    }
  }
}