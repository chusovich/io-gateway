void taskMqttMessenger(void *) {
  message_t message;
  for (;;) {
    if (mqttQueue.receiveMessage(&message)) {  // check if we have a message waiting for us, never timeout
      switch (message.id) {
        case 0:
          // pub
          break;
        case 1:
          //sub;
          break;
        case 2:
          // unsub
          break;
      }
    }
  }
}