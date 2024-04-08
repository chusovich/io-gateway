#define QUEUE_DATA_BUFFER_SIZE 100
#include <freeRTOS_pp.h>
#include "WiFi.h"
#include <PubSubClient.h>
#include <menuBuilder.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "secret.h"
#define ARDUINOJSON_ENABLE_ARDUINO_STREAM 1
#include <ArduinoJson.h>

// ----------- json declarations ----------- //
// json docs are declared in the tasks

// ----------- MQTT objects ----------- //
WiFiClient espClient;
PubSubClient client(espClient);

// ----------- freeRTOS objects ----------- //
#define PRO_CORE 0
#define APP_CORE 1
Task wifiManager("WiFi Manager", 4096, 1);
Task mqttManager("MQTT Cnct", 8192, 1);
Task mqttMessenger("MQTT Msger", 4096, 1);
Task serialReader("Serial Read Task", 4096, 1);
Task displayManager("Display Update Task", 4096, 0);
Queue mqttQueue(25);
Queue displayQueue(10);
Timer displayTimeout;

void taskWifiManager(void *);
void taskmqttManager(void *);
void taskMqttMessenger(void *);
void taskSerialReader(void *);
void taskDisplay(void *);
void timerCallback(TimerHandle_t timerHandle);

// ----------- display objects ----------- //
#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void refresh();
void createMenus();
Line Title(0, 32, 100, "MQTT Module"), WifiStatus(8, 0, 101, "WiFi: disconnected"), MqttStatus(16, 0, 102, "MQTT: disconnected"), RefreshButton(24, 0, 103, refresh, "Resubscribe");
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
  delay(2000);
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, D8, D10);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  display.clearDisplay();
  display.display();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);  // Start at top-left corner
  display.print("Launching...");
  display.display();
  // start up tasks and queues
  displayQueue.create();
  mqttQueue.create();
  displayTimeout.create("timer", 10000, false, 1, timerCallback);
  wifiManager.create(taskWifiManager, PRO_CORE);
  mqttManager.create(taskMqttManager, PRO_CORE);
  mqttMessenger.create(taskMqttMessenger, PRO_CORE);
  serialReader.create(taskSerialReader, APP_CORE);
  displayManager.create(taskDisplay, APP_CORE);
  // other setup
  createMenus();
  setupEncoderPins();
  vTaskDelete(NULL);
}

void loop() {
}