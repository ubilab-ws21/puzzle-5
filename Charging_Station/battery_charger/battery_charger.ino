/******
 * Puzzle 5
 * Battery charger in the safe box
 */
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <LiquidCrystal.h>

#define DEBUG_LCD 1
#define DEBUG_OFFLINE 0

// Puzzle setup
uint8_t BATTERY_UID[4] = {0xB3, 0x5B, 0xF2, 0xBB};

// Network configurations
#define WLAN_SSID "ssid"
#define WLAN_PASS "passphase"
#define WLAN_TIMEOUT 30

// MQTT configurations
#define MQTT_CLIENT_ID "Puzzle-5-charger"
#define MQTT_BROKER_IP "BROKER_IP"
#define MQTT_PORT 1883
#define MQTT_USERNAME "USERNAME"
#define MQTT_PASSWD "PASSWORD"
#define MQTT_TOPIC_BTY_LV "5/battery/1/level"
#define MQTT_TOPIC_BTY_LOC "5/battery/1/location"
#define MQTT_TOPIC_BTY_UID "5/battery/1/uid"
const char* mqtt_sub_topics[] = {
  MQTT_TOPIC_BTY_LV,
  MQTT_TOPIC_BTY_LOC,
  MQTT_TOPIC_BTY_UID
};
WiFiClient mqttClient;
PubSubClient mqtt(mqttClient);

// NFC reader pinout (SPI mode)
#define PN532_SCK  (18)
#define PN532_MOSI (23)
#define PN532_SS   (5)
#define PN532_MISO (19)
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

// LCD
// Note: GPIO12 on ESP32 NodeMCU may not work
// The initializer of LiquidCrystal cotain begin(1, 16)
// if it is called before setup(), ESP32 NodeMCU may crash

// Set pin to -1 if not connected
const int LCD_rs = 14, LCD_rw = 13, LCD_en = 27, LCD_d4 = 26, LCD_d5 = 25, LCD_d6 = 33, LCD_d7 = 32, LCD_bl = -1;
const int LCD_cols = 20, LCD_rows = 4;
LiquidCrystal lcd(LCD_rs, LCD_rw, LCD_en, LCD_d4, LCD_d5, LCD_d6, LCD_d7);
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

// Game status variables
int BatteryLv = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("\nSerial: up");

  setupLCD();
  
#if !DEBUG_OFFLINE
  if(!setupWiFi(WLAN_TIMEOUT) || !setupMQTT()) {
    delay(2000);
    ESP.restart();
  }
#endif
  setupNFC();

  lcd_printDefMsg();
}

void loop() {
  // Handle NFC reading
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

#if !DEBUG_OFFLINE
  if(WiFi.status() != WL_CONNECTED) { // Restart the board if WiFi is lost
#if DEBUG_LCD
    lcd.clear();
    lcd.print("WiFi disconnect");
#endif
    delay(2000);
    ESP.restart();
  }

  if(!mqtt.connected()) {  // Reconnect MQTT if connection is lost
#if DEBUG_LCD
    lcd.clear();
    lcd.print("Reconnect MQTT");
#endif
    setupMQTT();
  }
  mqtt.loop();
#endif

  Serial.println("\n\nPlace a card to read!");
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 3000);  // timeout=0 -> blocking
  if(success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);

    if(memcmp(BATTERY_UID, uid, 4) == 0) {
      Serial.println("Detected the battery!");

      // Start charging
      if(BatteryLv < 100)
        BatteryLv += 10;

      if(BatteryLv > 100)
        BatteryLv = 100;  // In case MQTT set it to a strange value
        
        // Publish battery level
#if !DEBUG_OFFLINE
        mqtt.publish(MQTT_TOPIC_BTY_LV, (byte*)&BatteryLv, 1, true);
#endif

      lcd_printBatteryLv();
    }
    delay(500); // slow down loop interval
  }
  else {
    // Terry: Found some cards will make the read function fail forever. Restart is needed.
    Serial.println("Failed to read a card! Try to reset.");
    setupNFC();
    lcd_printDefMsg();
  }
}

void setupNFC() {
  nfc.begin();

  // Try to get firmware version of the reader
  // Not guarantee to success in initial run!
//  uint32_t versiondata = nfc.getFirmwareVersion();
//  Serial.print("NFC chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
//  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
//  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

  // configure board to read RFID tags
  nfc.SAMConfig();
}

bool setupWiFi(unsigned int timeout) {
  unsigned int counter = timeout * 2;
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  Serial.printf("Connecting to SSID: %s", WLAN_SSID);
#if DEBUG_LCD
  lcd.clear();
  lcd.print("Connecting SSID:");
  lcd.setCursor(0, 1);
  lcd.print(WLAN_SSID);
  lcd.setCursor(0,2);
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

  if(counter) {
    Serial.printf("\nWiFi connected.\nIP address: \n%s\n", WiFi.localIP().toString());
    return true;
  }

  Serial.println("Wifi not connecting...");
  lcd.clear();
  lcd.println("WiFi failed");
  return false;
}

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
  if(res) {
    Serial.println("Connected to MQTT server");
    // Subscribe to topics
    mqtt.setCallback(mqttCallback);
    for(int i=0; i<sizeof (mqtt_sub_topics) / sizeof (const char *); i++) {
      Serial.printf("Subscribing to topic: %s\n", mqtt_sub_topics[i]);
      if(!mqtt.subscribe(mqtt_sub_topics[i])) {
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

void mqttCallback(char* topic, byte* message, unsigned int length)
{
  Serial.printf("Got %d byte(s) from %s\n", length, topic);

  // Topic handling
  if(strcmp(topic, MQTT_TOPIC_BTY_UID) == 0 && length == 4) {
    Serial.printf("Set battery's UID to: %x:%x:%x:%x", message[0], message[1], message[2], message[3]);
    for(int i=0; i<4; i++) {
      BATTERY_UID[i] = (uint8_t)message[i];
    }
  }
  else if (strcmp(topic, MQTT_TOPIC_BTY_LV) == 0 && length == 1) {
    Serial.printf("Set battery level to %d", message[0]);
    BatteryLv = message[0];
  }
  else {
    Serial.println("No handler for this message!");
  }
}

void setupLCD()
{
  lcd.begin(LCD_cols, LCD_rows);
  if(LCD_bl != -1)
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
  lcd.print("Place battery on");
  lcd.setCursor(0, 1);
  lcd.print("platform to charge!");
}

void lcd_printBatteryLv()
{
  int i = 0;
  lcd.clear();
  lcd.print("Battery Level:");
  lcd.setCursor(0, 1);
  if (BatteryLv == 0) {
    lcd.write(byte(LCD_LEFT_EDGE));
  }
  else {
    for(i=0; i<BatteryLv/10; i++) {
      lcd.write(byte(LCD_BLOCK));
    }
  }

  for(; i<9 ; i++) {
    lcd.write(byte(LCD_MID_EDGE));
  }
  if(BatteryLv < 100)
    lcd.write(byte(LCD_RIGHT_EDGE));
  else
    lcd.write(byte(LCD_BLOCK));

  lcd.printf(" %d%%", BatteryLv);

  lcd.setCursor(0, 2);
  if(BatteryLv >= 100)
    lcd.print("Fully charged!");
  else
    lcd.print("Keep charging!");
}
