// void taskSerialWriter(void *) {
//   message_t message;
//   Serial.println("write task init");
//   for (;;) {
//     if (serialQueue.receiveMessage(&message)) {  // never timeout
//       // The message was successfully received
//       Serial.println("message received");

//       Serial.print(">");
//       Serial.print(message.id);
//       Serial.print(":");
//       Serial.print(message.data);
//       Serial.print("!");

//     } else {
//       Serial.println("unable to receive the message from the queue");
//     }
//   }
// }
