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
  if(setupWiFi(WLAN_TIMEOUT))
    ESP.restart();

  setupNFC();
}

void loop() {
  // Handle NFC reading
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  if (success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
  }

  delay(1000);
}

void setupNFC() {
  nfc.begin();
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
  }

  Serial.println("Wifi not connecting...");
  return false;
}
