void createMenus() {
  HomeScreen.addLine(0, &Title);
  HomeScreen.addLine(1, &macAddress);
  HomeScreen.addLine(2, &numPeers);
  HomeScreen.addLine(3, &viewPeers);

  PeerView.addLine(0, &Home);
  PeerView.addLine(1, &Back);
  PeerView.addLine(2, &Next);
  PeerView.addLine(3, &Alias);
  PeerView.addLine(4, &PeerMac);
  PeerView.addLine(5, &Topics);
  PeerView.addLine(6, &Delete);

  MainMenu.addScreen(0, &HomeScreen);
  MainMenu.addScreen(1, &PeerView);
  MainMenu.selectScreen(0);
}

void timerCallback(TimerHandle_t timerHandle) {
  message_t timerMsg = { 3, "null" };
  displayQueue.enqueueISR(&timerMsg);
}

void incPeer() {
  message_t incPeerMsg = { 5, "null" };
  displayQueue.enqueueISR(&incPeerMsg);
}

void decPeer() {
  message_t decPeerMsg = { 6, "null" };
  displayQueue.enqueueISR(&decPeerMsg);
}

// set screen to 0
void goHome() {
  MainMenu.selectScreen(0);
  MainMenu.update();
}

void deletePeer() {
  message_t deleteMsg = { 8, "null" };
  displayQueue.enqueueISR(&deleteMsg);
}
void viewPeersScreen() {
  MainMenu.selectScreen(1);
  MainMenu.update();
}