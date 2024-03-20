#define QUEUE_DATA_BUFFER_SIZE 275
#include <freeRTOS_pp.h>
#include "WiFi.h"
#include <menuBuilder.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define ARDUINOJSON_ENABLE_ARDUINO_STREAM 1
#include <ArduinoJson.h>
#include "EspNowGateway.h"

// ----------- freeRTOS objects ----------- //
#define PRO_CORE 0
#define APP_CORE 1
Task espNowMessenger("ESP-NOW Messenger", 8192, 1);
Task serialReader("Serial Read Task", 4096, 1);
Task displayManager("Display Update Task", 4096, 0);
Queue displayQueue(10);
Timer displayTimeout;

void taskEspNowMessenger(void *);
void taskSerialReader(void *);
void taskDisplay(void *);

// ----------- ESP-NOW objects ----------- //
EspNowGateway gtw;
PeerData myPeerList[NUM_PEERS];

// ----------- display objects ----------- //
#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void createMenus();
void incPeer();
void decPeer();
void goHome();
void deletePeer();
void viewPeersScreen();
Line Title(0, 28, 100, "ESP-NOW Module"), macAddress(8, 0, 101, "MAC:XX:XX:XX:XX:XX:XX"), numPeers(16, 0, 102, "# Peers"), viewPeers(24, 0, 103, viewPeersScreen, "View Peers");
Line Home(8, 0, 104, goHome, "Return to Main Menu"), Back(16, 0, 105, decPeer, "< Prev"), Next(16, 90, 106, incPeer, "Next >");
Line Alias(24, 0, 201, "Peer Alias"), PeerMac(32, 0, 202, "MAC:XX:XX:XX:XX:XX:XX"), Topics(40, 0, 203, "View Topics"), Delete(48, 0, 204, deletePeer, "Delete Peer");
Screen HomeScreen(0, &display);
Screen PeerView(1, &display);
Menu MainMenu(0);

#define serialLED 9

// ----------- encoder setup ----------- //
volatile int clkPin = 1;
volatile int dtPin = 2;
int switchPin = 4;
volatile int count = 0;
volatile int clkPinLast = LOW;
volatile int clkPinCurrent = LOW;
static message_t isrMsg = { 1, "null" };
static message_t btnMsg = { 2, "null" };
void encoderInterrupt();
void switchInterrupt();
void setupEncoderPins();

// ----------- setup ----------- //
void setup() {
  delay(2000);
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  display.clearDisplay();
  display.display();
  // start up tasks and queues
  displayQueue.create();
  displayTimeout.create("timer", 5000, false, 1, timerCallback);
  espNowMessenger.create(taskEspNowMessenger, PRO_CORE);
  serialReader.create(taskSerialReader, APP_CORE);
  displayManager.create(taskDisplay, APP_CORE);
  // other setup
  createMenus();
  setupEncoderPins();
  vTaskDelete(NULL);
}

void loop() {
  // uxTaskGetStackHighWaterMark(NULL); // return in words so multiply by four 
  // xPortGetFreeHeapSize(); // return bytes
}