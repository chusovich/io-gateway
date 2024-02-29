void taskSerialRead(void *) {
  message_t mqttMessage;
  char* thing;
  Serial.println("read task init");
  for (;;) {
    if (reader.receiveCommand()) {
      Serial.println("command received");
      mqttMessage.id = reader.getLatestCommand();
      reader.getLatestCommandData(thing);

      if (mqttQueue.sendMessage(mqttMessage, 2000)) {
        // The message was successfully sent.
        Serial.println("msg sent");
      } else {
        Serial.println("send error");
      }
    } else {
    }
  }
}