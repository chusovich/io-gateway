void taskDisplay(void *) {
  message_t msg;
  MainMenu.focusLine(-1);
  MainMenu.update();
  for (;;) {
    displayQueue.dequeue(&msg);
    switch (msg.id) {
      case 1:  // inc line num
        if (count < 3 && count > 0) {
          MainMenu.focusLine(count);  // select line and update display
          MainMenu.update();
        } else if (count <= 0) {
          count++;
        } else {
          count--;
        }
        break;
      case 2:  // do action
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
    }
  }
}