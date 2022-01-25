/******
 * Puzzle 5
 * Battery charger in the safe box
 */
#include <WiFi.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <LiquidCrystal.h>

#define DEBUG 0
#define DEBUG_OFFLINE 0

// Puzzle setup
const uint8_t BATTERY_UID[4] = {0xB3, 0x5B, 0xF2, 0xBB};

// Network configurations
#define WLAN_SSID "ssid"
#define WLAN_PASS "passphase"
#define WLAN_TIMEOUT 30

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
const int LCD_rs = 14, LCD_rw = 13, LCD_en = 27, LCD_d4 = 26, LCD_d5 = 25, LCD_d6 = 33, LCD_d7 = 32;
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
  // LCD setup
  lcd.begin(20, 4);
  lcd.createChar(LCD_BLOCK, LCD_CHAR_BLOCK);
  lcd.createChar(LCD_LEFT_EDGE, LCD_CHAR_LEFT_EDGE);
  lcd.createChar(LCD_MID_EDGE, LCD_CHAR_MID_EDGE);
  lcd.createChar(LCD_RIGHT_EDGE, LCD_CHAR_RIGHT_EDGE);
  
  Serial.begin(115200);
  Serial.println("\nSerial: up");
#if !DEBUG_OFFLINE
  if(!setupWiFi(WLAN_TIMEOUT))
    ESP.restart();
#endif
  setupNFC();

  lcd_printDefMsg();
}

void loop() {
  // Handle NFC reading
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  Serial.println("\n\nPlace a card to read!");
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 2000);  // timeout=0 -> blocking
  if(success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);

    if(memcmp(BATTERY_UID, uid, 4) == 0) {
      Serial.println("Detected the battery!");

      // Start charging
      lcd_printBatteryLv();
      if(BatteryLv < 100)
        BatteryLv += 10;
      // TODO: publish current battery level
    }
  }
  else {
    // Terry: Found some cards will make the read function fail forever. Restart is needed.
    Serial.println("Failed to read a card! Try to reset.");
    setupNFC();
    lcd_printDefMsg();
  }

  delay(500);
}

void setupNFC() {
  nfc.begin();

  // Try to get firmware version of the reader
  // Not guarantee to success in initial run!
  uint32_t versiondata = nfc.getFirmwareVersion();
  Serial.print("NFC chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

  // configure board to read RFID tags
  nfc.SAMConfig();
}

bool setupWiFi(unsigned int timeout) {
  unsigned int counter = timeout * 2;
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  Serial.print("Connecting to SSID: ");
  Serial.print(WLAN_SSID);
  lcd.clear();
  lcd.home();
  lcd.print("Connecting SSID:");
  lcd.setCursor(0, 1);
  lcd.print(WLAN_SSID);
  lcd.setCursor(0,2);
  while(WiFi.status() != WL_CONNECTED && --counter) {
    Serial.print(".");
    lcd.print(".");
    delay(500);
  }

  lcd.clear();

  if(counter) {
    Serial.println("\nWiFi connected.\nIP address: ");
    Serial.println(WiFi.localIP());
    return true;
  }

  Serial.println("Wifi not connecting...");
  return false;
}

void lcd_printDefMsg()
{
  lcd.clear();
  lcd.home();
  lcd.print("Place battery on");
  lcd.setCursor(0, 1);
  lcd.print("platform to charge!");
}

void lcd_printBatteryLv()
{
  int i = 0;
  lcd.clear();
  lcd.home();
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

  lcd.print(" ");
  lcd.print(BatteryLv);
  lcd.print("%");

  lcd.setCursor(0, 2);
  if(BatteryLv >= 100)
    lcd.print("Fully charged!");
  else
    lcd.print("Keep charging!");
}