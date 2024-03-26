void taskSerialReader(void *) {
  JsonDocument doc;
  DeserializationError jsonError;
  message_t msg;
  Serial.flush();
  for (;;) {
    while (Serial1.available() == 0)
      vTaskDelay(20 / portTICK_PERIOD_MS);
    jsonError = deserializeJson(doc, Serial1);
    if (!jsonError) {
      doc.shrinkToFit();               // optional
      serializeJson(doc, msg.string);  // Serial.printf("message_t.string: %s\n", msg.string);
      mqttQueue.enqueue(msg, 1000);
    }
  }
}