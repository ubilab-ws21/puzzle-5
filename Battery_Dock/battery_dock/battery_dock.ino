/******
 * Puzzle 5
 * Battery dock at electric box
 */
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <LiquidCrystal.h>
#include <ArduinoJson.h>

#define DEBUG_LCD 1
#define DEBUG_OFFLINE 0

// Puzzle setup
uint8_t BATTERY_UID[4] = {0xB3, 0x5B, 0xF2, 0xBB};

// Network configurations
#define WLAN_SSID "ssid"
#define WLAN_PASS "passphase"
#define WLAN_TIMEOUT 30

// MQTT configurations
#define MQTT_CLIENT_ID "Puzzle-5-elec-box"
#define MQTT_BROKER_IP "BROKER_IP" 
#define MQTT_PORT 1883
#define MQTT_USERNAME "USERNAME"   // comment it if no authentication
#define MQTT_PASSWD "PASSWORD"    // comment it if no authentication
#define MQTT_TOPIC_BTY_LV "5/battery/1/level"
#define MQTT_TOPIC_BTY_LOC "5/battery/1/location"
#define MQTT_TOPIC_BTY_UID "5/battery/1/uid"
#define MQTT_TOPIC_CTL_PWR "5/control_room/power"
const char* mqtt_sub_topics[] = {
  MQTT_TOPIC_BTY_LV,
  MQTT_TOPIC_BTY_LOC,
  MQTT_TOPIC_BTY_UID,
  MQTT_TOPIC_CTL_PWR
};
WiFiClient mqttClient;
PubSubClient mqtt(mqttClient);

// NFC reader pinout (SPI mode)
#define PN532_SCK  (18)
#define PN532_MOSI (23)
#define PN532_SS   (5)
#define PN532_MISO (19)
//Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);  // Software SPI
Adafruit_PN532 nfc(PN532_SS);   // Hardware SPI

// LCD
// Note: GPIO12 on ESP32 NodeMCU may not work
// The initializer of LiquidCrystal cotain begin(1, 16)
// if it is called before setup(), ESP32 NodeMCU may crash

// Set pin to -1 if not connected
const int LCD_rs = 14, LCD_rw = -1, LCD_en = 27, LCD_d4 = 26, LCD_d5 = 25, LCD_d6 = 33, LCD_d7 = 32, LCD_bl = 13;
const int LCD_cols = 16, LCD_rows = 2;
LiquidCrystal lcd(LCD_rs, LCD_en, LCD_d4, LCD_d5, LCD_d6, LCD_d7);
byte LCD_CHAR_BLOCK[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte LCD_CHAR_LEFT_EDGE[8] = {
  B11111,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B11111,
};

byte LCD_CHAR_MID_EDGE[8] = {
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
};

byte LCD_CHAR_RIGHT_EDGE[8] = {
  B11111,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B11111,
};

enum {
  LCD_BLOCK,
  LCD_LEFT_EDGE,
  LCD_MID_EDGE,
  LCD_RIGHT_EDGE
};

// Game data definitions
enum BATTERY_LOC {
  LOC_CHARGER,
  LOC_ELEC_BOX,
  LOC_UNKNOWN = 255
};

// Game status variables
int BatteryLv = 0;
int BatteryLoc = LOC_UNKNOWN;
int CtrlPowerLv = 0;
int NFC_failCnt = 0;
bool MQTT_received = false;
bool CtrlRmPowerResumed = false;


/******************
 * Initalization
 *****************/

void setup() {
  Serial.begin(115200);
  Serial.println("\nSerial: up");

  setupLCD();

#if !DEBUG_OFFLINE
  if (!setupWiFi(WLAN_TIMEOUT) || !setupMQTT()) {
    delay(2000);
    ESP.restart();
  }
#endif
  setupNFC();

  lcd_printDefMsg();
}

/******************
 * Main Loop
 *****************/

void loop() {
  // Handle NFC reading
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

#if !DEBUG_OFFLINE
  if (WiFi.status() != WL_CONNECTED) { // Restart the board if WiFi is lost
#if DEBUG_LCD
    lcd.clear();
    lcd.print("WiFi disconnect");
#endif
    delay(2000);
    ESP.restart();
  }

  if (!mqtt.connected()) {  // Reconnect MQTT if connection is lost
#if DEBUG_LCD
    lcd.clear();
    lcd.print("Reconnect MQTT");
#endif
    setupMQTT();
  }
  mqtt.loop();
  if (MQTT_received) {
    // Skip to next cycle to handle all incoming MQTT messages first
    MQTT_received = false;
    return;
  }
#endif

  Serial.println("\n\nPlace a card to read!");
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 500);  // timeout=0 -> blocking
  if (success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);

    if (memcmp(BATTERY_UID, uid, 4) == 0) {
      int prevBatLv = BatteryLv;
      Serial.println("Detected the battery!");

      if (BatteryLv >= 10 && CtrlPowerLv < 100 && mqttPubBatLv(BatteryLv - 10)) {
        BatteryLv -= 10;
        CtrlPowerLv += 10;
        Serial.printf("Battery level: %d; Power level: %d\n", BatteryLv, CtrlPowerLv);
      }

      if (CtrlPowerLv >= 100 && !CtrlRmPowerResumed) {
        // Resume power of control room
        if (mqttPubCtrlRmPwr(true)) {
          CtrlRmPowerResumed = true;
          Serial.println("Control room power resumed!");
        } else {
          Serial.println("Failed to publish MQTT control room power trigger");
        }
      }

      // Update location
      if (BatteryLoc != LOC_ELEC_BOX) {
        Serial.println("Publish battery's location");
        if (mqttPubBatLoc(LOC_ELEC_BOX)) {
          BatteryLoc = LOC_ELEC_BOX;
        } else {
          Serial.println("Failed to publish location LOC_CHARGER");
        }
      }
      
      lcd_printPowerLv();
    }

    delay(1000); // slow down loop interval
  }
  else {
    NFC_failCnt++;
    if (NFC_failCnt >= 10) {
      Serial.println("No card is detected in the past 10 pulls");
      NFC_failCnt = 0;
     
     // Some cards will make the read function fail forever. Restart is needed.
     // Issue seems to be eliminated after resolving issue #80
     // This workaround is now unnecessary
     //setupNFC();
    }

    lcd_printDefMsg();

    // Set battery's location to LOC_UNKNOWN if it was here
    if (BatteryLoc == LOC_ELEC_BOX) {
      // publish location
      if (mqttPubBatLoc(LOC_UNKNOWN)) {
        BatteryLoc = LOC_UNKNOWN;
      } else {
        Serial.println("Failed to publish location LOC_UNKNOWN");
      }
    }
  }
}

/***************************
 * Functions - NFC related
 **************************/

void setupNFC() {
  nfc.begin();

  // Try to get firmware version of the reader
  // Not guarantee to success in initial run! Check issue #14, #80
  uint32_t versiondata = nfc.getFirmwareVersion();
  Serial.print("NFC chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

  // configure board to read RFID tags
  nfc.SAMConfig();
}

/***************************
 * Functions - WiFi related
 **************************/

bool setupWiFi(unsigned int timeout) {
  unsigned int counter = timeout * 2;
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  Serial.printf("Connecting to SSID: %s", WLAN_SSID);
#if DEBUG_LCD
  lcd.clear();
  lcd.print("Connecting SSID:");
  lcd.setCursor(0, 1);
  lcd.print(WLAN_SSID);
#endif
  while(WiFi.status() != WL_CONNECTED && --counter) {
    Serial.print(".");
#if DEBUG_LCD
    lcd.print(".");
#endif
    delay(500);
  }

#if DEBUG_LCD
  lcd.clear();
#endif

  if (counter) {
    Serial.printf("\nWiFi connected.\nIP address: \n%s\n", WiFi.localIP().toString().c_str());
    return true;
  }

  Serial.println("Wifi not connecting...");
  lcd.clear();
  lcd.println("WiFi failed");
  return false;
}

/***************************
 * Functions - MQTT related
 **************************/

bool setupMQTT()
{
  bool res;
  Serial.printf("Connecting to MQTT broker at %s:%d", MQTT_BROKER_IP, MQTT_PORT);
#if DEBUG_LCD
  lcd.clear();
  lcd.print("Connect MQTT");
  lcd.setCursor(0, 1);
  lcd.print(MQTT_BROKER_IP);
#endif

  mqtt.setServer(MQTT_BROKER_IP, MQTT_PORT);
#if (defined MQTT_USERNAME) && (defined MQTT_PASSWD)
  Serial.printf("Connecting with username: %s, password: %s\n", MQTT_USERNAME, MQTT_PASSWD);
  res = mqtt.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWD);
#else
  res = mqtt.connect(MQTT_CLIENT_ID);
#endif

#if DEBUG_LCD
  lcd.clear();
#endif
  if (res) {
    Serial.println("Connected to MQTT server");
    // Subscribe to topics
    mqtt.setCallback(mqttCallback);
    for(int i=0; i<sizeof (mqtt_sub_topics) / sizeof (const char *); i++) {
      Serial.printf("Subscribing to topic: %s\n", mqtt_sub_topics[i]);
      if (!mqtt.subscribe(mqtt_sub_topics[i])) {
        Serial.printf("Failed to subscribe to the topic!");
        return false;
      }
    }
    
  }
  else {
    Serial.println("Failed to connect to MQTT server");
    lcd.clear();
    lcd.println("MQTT failed!");
  }
  return res;
}

bool mqttPubBatLv(int batLv)
{
  StaticJsonDocument<200> doc;
  doc["method"] = "message";
  doc["data"] = batLv;
  char json[200];
  serializeJson(doc, json);
  return mqtt.publish(MQTT_TOPIC_BTY_LV, json, true);
}

bool mqttPubBatLoc(int loc)
{
  StaticJsonDocument<200> doc;
  doc["method"] = "message";
  doc["data"] = loc;
  char json[200];
  serializeJson(doc, json);
  return mqtt.publish(MQTT_TOPIC_BTY_LOC, json, true);
}

bool mqttPubCtrlRmPwr(bool state)
{
  StaticJsonDocument<200> doc;
  doc["method"] = "status";
  doc["state"] = state ? "solved" : "active";
  char json[200];
  serializeJson(doc, json);
  return mqtt.publish(MQTT_TOPIC_CTL_PWR, json, true);
}

void mqttCallback(char* topic, byte* message, unsigned int length)
{
  StaticJsonDocument<200> mqtt_decoder;
  Serial.printf("Got %d byte(s) from %s\n", length, topic);
  MQTT_received = true;

  if (deserializeJson(mqtt_decoder, message)) {
    Serial.println("Failed to deserialize JSON");
    return;
  }

  // Topic handling
  if (strcmp(topic, MQTT_TOPIC_BTY_UID) == 0) {
    // Assume string in format "aa:bb:cc:dd"
    sscanf(mqtt_decoder["data"], "%x:%x:%x:%x", &BATTERY_UID[0], &BATTERY_UID[1], &BATTERY_UID[2], &BATTERY_UID[3]);
    Serial.printf("Received battery's UID: %x:%x:%x:%x", BATTERY_UID[0], BATTERY_UID[1], BATTERY_UID[2], BATTERY_UID[3]);
  }
  else if (strcmp(topic, MQTT_TOPIC_BTY_LV) == 0) {
    int batLv = mqtt_decoder["data"]; // TODO: Error handling in case data doesn't exist
    Serial.printf("Received battery level: %d", batLv);
    BatteryLv = batLv;
  }
  else if (strcmp(topic, MQTT_TOPIC_BTY_LOC) == 0) {
    Serial.printf("Receive battery location: %d", BatteryLoc);
    BatteryLoc = mqtt_decoder["data"];
  }
  else if (strcmp(topic, MQTT_TOPIC_CTL_PWR) == 0 && 
           strcmp(mqtt_decoder["method"], "status") == 0) {
    if (strcmp(mqtt_decoder["state"], "active") == 0) {
      // Reset the game
      CtrlPowerLv = 0;
      CtrlRmPowerResumed = false;
    }
  }
  else {
    Serial.println("No handler for this message!");
  }
}

/***************************
 * Functions - LCD related
 **************************/

void setupLCD()
{
  lcd.begin(LCD_cols, LCD_rows);
  if (LCD_bl != -1)
  {
    pinMode(LCD_bl, OUTPUT);
    digitalWrite(LCD_bl, HIGH);  // backlight defualt on
  }
  lcd.createChar(LCD_BLOCK, LCD_CHAR_BLOCK);
  lcd.createChar(LCD_LEFT_EDGE, LCD_CHAR_LEFT_EDGE);
  lcd.createChar(LCD_MID_EDGE, LCD_CHAR_MID_EDGE);
  lcd.createChar(LCD_RIGHT_EDGE, LCD_CHAR_RIGHT_EDGE);
}

void lcd_backLight(bool on)
{
  digitalWrite(LCD_bl, on ? HIGH:LOW);
}

void lcd_printDefMsg()
{
  lcd.clear();
  if (CtrlRmPowerResumed && BatteryLoc != LOC_ELEC_BOX) {
    lcd_backLight(true);
    // Current logic: removing fully charged battery won't drop the power of control room
    lcd.print("Keep the battery");
    lcd.setCursor(0,1);
    lcd.print("here!");
  }
  else {
    // Assume no power
    lcd_backLight(false);
  }
}

void lcd_printPowerLv()
{
  int i = 0;
  
  lcd.clear();
  lcd_backLight(true);
  lcd.setCursor(0, 1);
  if (CtrlPowerLv == 0) {
    lcd.write(byte(LCD_LEFT_EDGE));
  }
  else {
    for(i=0; i<CtrlPowerLv/10; i++) {
      lcd.write(byte(LCD_BLOCK));
    }
  }

  for(; i<9 ; i++) {
    lcd.write(byte(LCD_MID_EDGE));
  }
  if (CtrlPowerLv < 100)
    lcd.write(byte(LCD_RIGHT_EDGE));
  else
    lcd.write(byte(LCD_BLOCK));

  lcd.printf(" %d%%", CtrlPowerLv);

  lcd.home();
  if (CtrlPowerLv == 0) {
    // Blink power critial message
    lcd.print("Power critial!");
    for(int i=0; i<2; i++) {
      delay(500);
      lcd.noDisplay();
      delay(500);
      lcd.display();
    }
    lcd_backLight(false);
    lcd.clear();
  }
  else if(CtrlPowerLv >= 100) {
    lcd.print("Discharging...");
  }
  else if(BatteryLv > 0) {  // Transferring power from battery
    lcd.print("Transferring...");
  }
  else {
    lcd.print("Please recharge!");
  }
  
}
