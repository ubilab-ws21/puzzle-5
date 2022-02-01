#include <Arduino.h>
#include "display.h"
#include "lvgl.h"
#include "lv_conf.h"
//#include "wifi_mqtt.cpp"
//#include "imu.h"
//#include "rgb_led.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <PubSubClient.h>
#include "lv_port_indev.h"
#include "lv_cubic_gui.h"
#include "lv_demo_encoder.h"
#include "game2048.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "common.h"
#include <time.h>
#include <BLE.h>
#include "app_controller.h"
#include "app_controller_gui.h"
#include "game2048_controller.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <PubSubClient.h>

//#include <WiFi.h>
//#include <ESPmDNS.h>
//#include <PubSubClient.h>

//#define SERVER_IP "192.168.1.113" // IP or DNS name
//#define SERVER_PORT 2000 // Port >= 2000
//#define MQTT_SERVER_IP "192.168.1.113" // MQTT Server IP/hostname
//#define MQTT_SERVER_IP "192.168.137.1"
//#define MQTT_PORT 1883 // Port of MQTT Server
//#define SSID "whatever"
//#define PWD "youlike!!" 
/*** Component objects ***/

#define SERVER_IP "192.168.1.113" // IP or DNS name
#define SERVER_PORT 2000 // Port >= 2000
WiFiClient client;

//#define MQTT_SERVER_IP "192.168.1.113" // MQTT Server IP/hostname
#define MQTT_SERVER_IP "192.168.137.1"
#define MQTT_PORT 1883 // Port of MQTT Server

WiFiClient mqttClient;
PubSubClient mqtt(mqttClient);

#define NAME "Puzzle0"

/* terry creats wifi by phone(makes computer has 2.4GHz wifi not 5Ghz)，amd my computer creates
a mew wifi named below for esp32 to connect(2.4Ghz) and then fill the debugger with the given mqtt server ip from 
up(it is gotten from my computer ipconfig)because now my computer now is the server for locals, later i can change 
it into the server ip where escape room has */
/* #define SSID "whatever"
#define PWD "youlike!!" */

#define SSID "whatever"
#define PWD "youlike!!" 


Display screen;
//IMU mpu;
BLE_Action *act_info;          // 存放mpu6050返回的数据
AppController *app_contorller; // APP控制器
BLE ble;
extern String rxload;
#ifdef __cplusplus
 //WiFiClient client;
//extern WiFiClient mqttClient;
//extern PubSubClient mqtt(mqttClient);
#endif 
/* terry creats wifi by phone(makes computer has 2.4GHz wifi not 5Ghz)，amd my computer creates
a mew wifi named below for esp32 to connect(2.4Ghz) and then fill the debugger with the given mqtt server ip from 
up(it is gotten from my computer ipconfig)because now my computer now is the server for locals, later i can change 
it into the server ip where escape room has */
/* #define SSID "whatever"
#define PWD "youlike!!" */
//void mqttCallback(char* topic, byte* message, unsigned int length);
void mqttCallback(char* topic, byte* message, unsigned int length);
void setup()
{
    Serial.begin(115200);
   
    /*mqtt configure*/
    Serial.print("Connecting to ");
    Serial.println(SSID);
     WiFi.setHostname(NAME);
    WiFi.begin(SSID, PWD);
    // Wait while not connected
     while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500); 
        Serial.print(".");
    }
    // Print IP
    Serial.println("\nWiFi connected.\nIP Adress: ");
    Serial.println(WiFi.localIP());
    
    // Setting up mDNS
    if (!MDNS.begin(NAME)) 
    {
     Serial.println("Error setting up MDNS responder!");
    }
    MDNS.addService("_escape", "_tcp", 2000);
    // Connect the client
    if (client.connect(SERVER_IP, SERVER_PORT))
    {
        Serial.println("Connected to server");
        client.println("welcome Puzzle 5");
    }
    else {
        Serial.println("Cannot connect to server");
    }

    mqtt.setCallback(mqttCallback);
    mqtt.setServer(MQTT_SERVER_IP, MQTT_PORT);
    // MQTT connect
    if (mqtt.connect(NAME)) 
    {
        Serial.println("Connected to MQTT server");
        // Publish a message
        const char * topic = "Puzzle_5_status:";
        bool retained = false;
        mqtt.publish(topic, "5/safe/control", retained);
    } 
    else {
        Serial.println("Cannot connect to MQTT server");
    }
    mqtt.subscribe("5/safe/feedback"); 
    /*end of mqtt configure */
    
    ble.setupBLE("Escape_Room");//setup bluetooth name
    randomSeed(2);//analogRead(25)
    /*** Init screen ***/
	screen.init();
	screen.setBackLight(0.2);
    Serial.println("Setup done");

    lv_port_indev_init();
	/*** Init on-board RGB ***/
    app_contorller = new AppController();
	app_contorller->app_register(&game_2048_app1);
    app_contorller->app_register(&game_2048_app);
    app_contorller->main_process(&ble.action_info);
}

void loop(){
    //mqtt.loop(); // Only required if you subscribe to sth
    //mqtt.loop();
	screen.routine();
    act_info = ble.BLEAction(rxload);
    app_contorller->main_process(act_info); // 运行当前进程
}

#define MAX_MSG 50
char msg[MAX_MSG] = {'\0'};
// MQTT Callback function
void mqttCallback(char* topic, byte* message, unsigned int length) {
    // Convert to cstring
    int len = min((int)length, (int)(MAX_MSG-1));
    memcpy(&msg[0], message, len);
    msg[len] = '\0';
    // Print incoming msg
    Serial.printf("MQTT Message on topic: %s: %s\n", topic, &msg);
}