void taskDisplay(void *) {
  message_t msg;
  int c = 0;
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
      case 4:
        // update display
        MainMenu.update();
        break;
      case 5:  // inc peer
        if (c < NUM_PEERS - 1) {
          c++;
        }
        // PeerMac.text = peerInfo[c].mac;
        // Alias.text = peerInfo[c].alias;
        break;
      case 6:  // dec peer
        if (c > -1) {
          c--;
        }
        break;
    }
  }
}