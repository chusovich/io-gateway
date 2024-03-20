void taskSerialReader(void *) {
  pinMode(serialLED, OUTPUT);
  digitalWrite(serialLED, LOW);
  JsonDocument doc;
  DeserializationError jsonError;
  message_t msg;
  Serial.flush();
  for (;;) {
    while (Serial.available() == 0) {}
    digitalWrite(serialLED, HIGH);
    vTaskDelay(20 / portTICK_PERIOD_MS);
    jsonError = deserializeJson(doc, Serial);
    if (!jsonError) {
      doc.shrinkToFit();               // optional
      serializeJson(doc, msg.string);  // Serial.printf("message_t.string: %s\n", msg.string);
      mqttQueue.enqueue(msg, 1000);
    }
    digitalWrite(serialLED, LOW);
  }
}