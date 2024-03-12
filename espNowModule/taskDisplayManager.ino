void taskDisplay(void *) {
  message_t msg;
  int c = 0;
  char numPeersStr[15] = "# of Peers: ";
  char macStr[25];
  int peerCount = 0;
  MainMenu.focusLine(-1);
  MainMenu.update();

  for (;;) {
    displayQueue.dequeue(&msg);
    switch (msg.id) {
      case 1:  // inc line num
        displayTimeout.reset(100);
        if (count < 7 && count > -1) {
          MainMenu.focusLine(count);  // select line and update display
          MainMenu.update();
        } else if (count <= 0) {
          count++;
        } else {
          count--;
        }
        break;
      case 2:  // do action
        displayTimeout.reset(100);
        MainMenu.doAction();
        break;
      case 3:
        // user timeout
        MainMenu.focusLine(-1);
        MainMenu.update();
        break;
      case 4:  // update gateway MAC address
        macAddress.text = msg.string;
        MainMenu.update();
        break;
      case 5:  // inc peer
        if (c < NUM_PEERS - 1) {
          c++;
        }
        sprintf(macStr, "MAC:%02X:%02X:%02X:%02X:%02X:%02X", myPeerList[c].mac[0], myPeerList[c].mac[1], myPeerList[c].mac[2], myPeerList[c].mac[3], myPeerList[c].mac[4], myPeerList[c].mac[5]);
        PeerMac.text = macStr;
        Alias.text = myPeerList[c].alias;
        MainMenu.update();
        break;
      case 6:  // dec peer
        if (c > -1) {
          c--;
        }
        sprintf(macStr, "MAC:%02X:%02X:%02X:%02X:%02X:%02X", myPeerList[c].mac[0], myPeerList[c].mac[1], myPeerList[c].mac[2], myPeerList[c].mac[3], myPeerList[c].mac[4], myPeerList[c].mac[5]);
        PeerMac.text = macStr;
        Alias.text = myPeerList[c].alias;
        MainMenu.update();
        break;
      case 7:
        peerCount = 0;
        for (int p = 0; p < NUM_PEERS; p++) {
          if (myPeerList[p].active) {
            peerCount++;
          }
        }
        sprintf(numPeersStr, "# of Peers: %d", peerCount);
        numPeers.text = numPeersStr;
        MainMenu.update();
        break;
    }
  }
}