/*
 This program tests the Holocube Core, Wired and Wireless interfaces
 Remember to copy pin_definitions.h into soc_test folder or run ../header.sh -c
*/

// Enable either WF or BL to test WiFi or BT/BLE Radio
#define WF
//#define BL

#include "pin_definitions.h"

#ifdef WF
#include <WiFi.h>
WiFiServer server(80);
#endif

#ifdef BL
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
#endif

const char* st_ssid = "ubilab_wifi(_5G)";
const char* st_pwd = "ohg4xah3oufohreiPe7e";
const char* ap_ssid = "Holocube";
const char* ap_pwd = "puzzle5";
const char* bluetooth_device_name = "Holocube";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("\n\nTesting the ESP32-PICO-D4 SiP's UART-USB, WiFi & BT/BLE interfaces\n\n");
#ifdef WF
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("As a STATION: Connecting to ");
  Serial.println(st_ssid);

  // WiFi Station (ST) Test
  WiFi.begin(st_ssid, st_pwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("WiFi ST test --> passed");
  delay(2000);
  Serial.print("Disconnecting...");
  WiFi.disconnect();
  Serial.println("WiFi Station Disconnected.");

  // WiFi Access Point (AP) Test
  delay(2000);
  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ap_ssid, ap_pwd);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("Server started");
  WiFiClient client = server.available();
  while (1) {
    WiFiClient client = server.available();   // listen for incoming clients

    if (client) {                             // if you get a client,
      Serial.println("New client connected!");           // print a message out the serial port
      while (client.connected()) {            // loop while the client's connected
        if (client.available()) {
          if (client.readString() == "continue") {
            client.println("WiFi AP test --> passed");
            Serial.println("WiFi AP test --> passed");
            client.flush();
            delay(100);
            client.stop();
            server.stop();
            delay(2000);
            Serial.print("Disconnecting...");
            WiFi.disconnect();
            Serial.println("WiFi Access Point Disconnected.");
            break;
          }
        }
      }
    }
  }
#endif
#ifdef BL
  // Bluetooth Classic (BT) and Bluetooth Low Energy (BLE) Tests
  SerialBT.begin(bluetooth_device_name);
  Serial.printf("The device (%s) started, now you can pair it with bluetooth!", bluetooth_device_name);
#endif
}

void loop() {
  // put your main code here, to run repeatedly:
#ifdef BL
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  delay(20);
  #endif
}
