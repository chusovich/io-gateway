void TaskSerialRead(void *) {
  StreamReader reader(&Serial, 2);
  message_t message;
  Serial.println("read task init");
  for (;;) {
    if (reader.receiveCommand()) {
      Serial.println("command received");
      message.name = reader.getLatestCommand();
      message.data = reader.getLatestCommandData();

      if (incomingMsgs.sendMessage(message, 2000)) {
        // The message was successfully sent.
        Serial.println("msg sent");
      } else {
        Serial.println("send error");
      }
    } else {
    }
  }
}