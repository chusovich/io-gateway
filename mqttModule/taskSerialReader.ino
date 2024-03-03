void taskSerialReader(void *) {
  JsonDocument doc;
  DeserializationError jsonError;
  message_t msg;
  for (;;) {
    while (Serial.available() == 0)
      vTaskDelay(20 / portTICK_PERIOD_MS);
    jsonError = deserializeJson(doc, Serial);
    if (jsonError) {
      Serial.printf("deserializeJson() failed: %s\n", jsonError.c_str());
    } else {
      doc.shrinkToFit();  // optional
      serializeJson(doc, msg.string); //Serial.printf("message_t.string: %s\n", msg.string);
      mqttQueue.enqueue(msg, 1000);
    }
  }
}