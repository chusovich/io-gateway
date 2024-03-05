void createMenus() {
  HomeScreen.addLine(0, &Title);
  HomeScreen.addLine(1, &macAddress);
  HomeScreen.addLine(2, &numPeers);
  HomeScreen.addLine(3, &viewPeers);
  MainMenu.addScreen(0, &HomeScreen);
  MainMenu.selectScreen(0);
}