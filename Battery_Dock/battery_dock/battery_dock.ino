/******
 * Puzzle 5
 * Battery dock at electric box
 */
#include <WiFi.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

#define DEBUG 1
#define DEBUG_OFFLINE 0

// Puzzle setup
const uint8_t BATTERY_UID[4] = {0xFE, 0x88, 0x50, 0x10};

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

void setup() {
  Serial.begin(115200);
  Serial.println("\nSerial: up");
#if !DEBUG_OFFLINE
  if(!setupWiFi(WLAN_TIMEOUT))
    ESP.restart();
#endif
  setupNFC();
}

void loop() {
  // Handle NFC reading
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  Serial.println("\n\nPlace a card to read!");
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 0);  // timeout=0 -> blocking
  if(success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);

    if(memcmp(BATTERY_UID, uid, 4) == 0) {
      Serial.println("Detected the battery!");
    }
  }
  else {
    // Terry: Found some cards will make the read function fail forever. Restart is needed.
    Serial.println("Failed to read a card! Try to reset.");
    setupNFC();
  }

  delay(1000);
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
  while(WiFi.status() != WL_CONNECTED && --counter) {
    Serial.print(".");
    delay(500);
  }

  if(counter) {
    Serial.println("\nWiFi connected.\nIP address: ");
    Serial.println(WiFi.localIP());
    return true;
  }

  Serial.println("Wifi not connecting...");
  return false;
}
