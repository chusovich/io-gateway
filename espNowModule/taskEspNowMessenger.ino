void taskEspNowMessenger(void *) {
  message_t msg;
  JsonDocument doc;
  DeserializationError jsonError;

  for (;;) {
    espNowQueue.peek(&msg);  // see if we have a message
    jsonError = deserializeJson(doc, msg.string);
    // Serial.printf("message received: %s\n", msg.string);
    if (jsonError) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(jsonError.c_str());
      espNowQueue.dequeue(&msg);
    } else {
      doc.shrinkToFit();  // optional
      char output[300];
      serializeJson(doc, output);
      String topic;
      switch (doc["id"].as<int>()) {
        case 1:  // send message
          topic = String(doc["topic"]);
          for (int i = 0; i <= NUM_PEERS - 1; i++) {  // for each peer...
            if (peerList[i].active) {                 // if it is active...
              // Serial.print("Active Mac: ");
              Serial.println(peerList[i].mac[0]);
              for (int j = 0; j <= NUM_TOPICS; j++) {  // search through all 12 topics
                // Serial.println("Searching Topics...");
                if (peerList[i].topics[j].equals(topic)) {  // if we find a match...
                  // create the message buffer and send the message
                  WifiEspNow.send(peerList[i].mac, reinterpret_cast<const uint8_t *>(output), strlen(output));
                  Serial.println("Message Sent!");
                }
              }
            }
          }
          break;
        case 2:  // refresh
          {
            JsonDocument jsonDoc;
            doc["id"] = 2;
            for (int i = 0; i <= NUM_PEERS; i++) {
              for (int j = 0; j <= NUM_TOPICS; j++) {
                if (peerList[i].topics[j].indexOf("/") != -1) {
                  jsonDoc["topic"] = peerList[i].topics[j];
                  serializeJson(jsonDoc, Serial);
                }
              }
            }
          }
          break;
        case 3:  // add peer
          Serial.println("Add case");
          const uint8_t mac[6] = { 0, 0, 0, 0, 0, 0 };
          addPeerToList(mac);  // add peer to our peer list
          WifiEspNow.addPeer(mac);
          break;
        case 4:  // sub
          const uint8_t mac[6] = { 0, 0, 0, 0, 0, 0 };
          addTopicToPeer(mac, topic);
          serializeJson(doc, Serial);
          break;
        case 4:                        // sub
          serializeJson(doc, Serial);  // tell mqtt to publish the topic and payload
          break;
      }  // switch statement
    }    // else
  }      // infinite loop
}