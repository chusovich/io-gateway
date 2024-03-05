#define QUEUE_DATA_BUFFER_SIZE 275
#include <freeRTOS_API.h>
#include "WiFi.h"
#include <WifiEspNow.h>
#include <menuBuilder.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <StreamUtils.h>
#define ARDUINOJSON_ENABLE_ARDUINO_STREAM 1
#include <ArduinoJson.h>

// ----------- json declarations ----------- //
ReadBufferingStream bufferingStream(Serial, 150);
// json docs are declared in the tasks

// ----------- ESP-NOW objects ----------- //
#define NUM_PEERS 10
#define NUM_TOPICS 12
struct peerInfo {
  bool active = false;
  uint8_t mac[6] = { 0, 0, 0, 0, 0, 0 };
  String topics[NUM_TOPICS];
};
peerInfo peerList[NUM_PEERS];

// ----------- freeRTOS objects ----------- //
#define PRO_CORE 0
#define APP_CORE 1
Task espNowMessenger("ESP-NOW Messenger", 4096, 1);
Task serialReader("Serial Read Task", 4096, 1);
Task displayManager("Display Update Task", 4096, 0);
Queue espNowQueue(25);
Queue displayQueue(10);

void taskEspNowMessenger(void *);
void taskSerialReader(void *);
void taskDisplay(void *);

// ----------- display objects ----------- //
#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void createMenus();
Line Title(0, 32, 100, "ESP-NOW Module"), macAddress(8, 0, 101, "MAC:XX:XX:XX:XX:XX:XX"), numPeers(16, 0, 102, "# Peers"), viewPeers(24, 0, 103, "View Peers");
Screen HomeScreen(0, &display);
Menu MainMenu(0);

// ----------- encoder setup ----------- //
volatile int clkPin = 2;
volatile int dtPin = 3;
int switchPin = 5;
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
  Serial.begin(115200);
  espNowSetup();
  // init OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  display.clearDisplay();
  display.display();
  // start up tasks and queues
  espNowQueue.create();
  displayQueue.create();
  espNowMessenger.createTask(taskEspNowMessenger, PRO_CORE);
  serialReader.createTask(taskSerialReader, APP_CORE);
  displayManager.createTask(taskDisplay, APP_CORE);

  // other setup
  createMenus();
  setupEncoderPins();
  vTaskDelete(NULL);
}

void loop() {
  // code never gets here
}