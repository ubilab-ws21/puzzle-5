#include <Arduino.h>
#include "display.h"
#include "lvgl.h"
#include "lv_conf.h"
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
#include "app_switch.h"
#include "app_switch_gui.h"
#include "game2048_controller.h"

/*** Component objects ***/
Display screen;
BLE_Action *act_info; 
AppController *app_contorller; // APP控制器
BLE ble;
extern String rxload;
/*end of configuration of BLE
1)BLE initialize
2)backlight setting
3)lvgl gui configuration*/
void setup()
{
    Serial.begin(115200);
    ble.setupBLE("Escape_Room");//setup bluetooth name
    randomSeed(2);
	
    /*** Init screen ***/
	screen.init();
	screen.setBackLight(0.2);
    
	lv_port_indev_init();
    app_contorller = new AppController();
	app_contorller->app_register(&escape_room);
    app_contorller->app_register(&game_2048_app);
	app_contorller->main_process(&ble.action_info);
}



void loop(){

	screen.routine();
    act_info = ble.BLEAction(rxload);
    app_contorller->main_process(act_info); 
}