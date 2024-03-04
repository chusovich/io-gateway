void encoderInterrupt() {
  clkPinCurrent = digitalRead(clkPin);
  if ((clkPinLast == LOW) && (clkPinCurrent == HIGH)) {
    if (digitalRead(dtPin) == HIGH) {
      count--;
    } else {
      count++;
    }
    displayQueue.enqueueISR(&isrMsg);
  }
  clkPinLast = clkPinCurrent;
}

void switchInterrupt() {
  displayQueue.enqueueISR(&btnMsg);
}

void setupEncoderPins() {
  pinMode(clkPin, INPUT);
  pinMode(dtPin, INPUT);
  pinMode(switchPin, INPUT_PULLDOWN);
  attachInterrupt(clkPin, encoderInterrupt, CHANGE);
  attachInterrupt(dtPin, encoderInterrupt, CHANGE);
  attachInterrupt(switchPin, switchInterrupt, RISING);
}