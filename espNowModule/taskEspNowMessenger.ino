void taskEspNowMessenger(void *) {
  message_t msg;
  JsonDocument doc;
  JsonArray jsonMac;
  DeserializationError jsonError;
  char jsonString[300];
  uint8_t macAddr[6];
  gtw.begin();
  gtw.setQueue(&espNowQueue);

  for (;;) {
    espNowQueue.peek(&msg);  // see if we have a message
    jsonError = deserializeJson(doc, msg.string);
    if (jsonError) {
      Serial.printf("deserializeJson() failed: %s\n", jsonError.c_str());
    } else {
      espNowQueue.dequeue(&msg);
      serializeJson(doc, jsonString); // Serial.printf("dequeued msg: %d", doc["id"].as<int>());
      switch (doc["id"].as<int>()) {
        case 1:  // send message - expect "topic" and "payload" objects in json doc
          gtw.forwardMessageToPeers(doc["topic"], doc["payload"]);
          break;
        case 2:  // refresh - no json data need
          gtw.refresh();
          break;
        case 3:  // add peer - "mac" object expected in json doc, "id" is reused
          jsonMac = doc["mac"];
          for (int i = 0; i < 6; i++) {
            macAddr[i] = jsonMac[i];
          }
          gtw.addPeer(macAddr);  // add peer to our peer list
          break;
        case 4:  // sub - expected "topic" and "mac" objects in json doc, "id" is reused
          jsonMac = doc["mac"];
          for (int i = 0; i < 6; i++) {
            macAddr[i] = jsonMac[i];
          }
          gtw.subPeerToTopic(macAddr, doc["topic"]);
          serializeJson(doc, Serial);  // tell mqtt to sub to this topic
          break;
        case 5:                        // pub - "topic" and "payload" objects expected, id is reused
          serializeJson(doc, Serial);  // tell mqtt to publish the topic and payload
          break;

      }  // switch statement
    }    // else
  }      // infinite loop
}