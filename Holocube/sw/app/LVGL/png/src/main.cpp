#include <Arduino.h>
#include "display.h"
#include "lvgl.h"
#include "lv_gif.h"
#include "rgb_led.h"
#include "lv_conf.h"
#include "lv_port_indev.h"
#include "lv_cubic_gui.h"
//#include "lv_demo_encoder.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "common.h"
#include <time.h>
//#include "imu.h"
//#include "ambient.h"
//#include "network.h"
//#include "sd_card.h"
//#include "gui_guider.h"
//#include "lv_port_fatfs.h"
//#include <lv_png.h>
//#include "lodepng.h"

/*** Component objects ***/
Display screen;
//IMU mpu;
Pixel rgb;
//SdCard tf;
//Network wifi;

//lv_ui guider_ui;
//GAME2048 game;
//int *p;
//int *m;
//long time1;

/*LVGL task one*/
void taskOne( void * parameter ){
  while(1){
	lv_tick_inc(5);
	delay(5);
  }
  Serial.println("Ending task 1");
  vTaskDelete( NULL );
}
/*LVGL task two_ main function of LVGL*/
void taskTwo(void * parameter){
	while(1){
		screen.routine();
		delay(5);
	}
  Serial.println("Ending task 2");
  vTaskDelete( NULL );
}

/* static void my_lvgl_test(void)
{
  printf( "LVGL gif now opened");

	  lv_obj_t * imgbtn = lv_imgbtn_create( lv_scr_act(), NULL );
	
  	lv_imgbtn_set_src( imgbtn, LV_BTN_STATE_PRESSED, "S:/2048/2048.bin" );

  	lv_imgbtn_set_src( imgbtn, LV_BTN_STATE_RELEASED, "S:/2048/2048.bin" );
  	
  	lv_obj_align( imgbtn, NULL, LV_ALIGN_CENTER, 0, 0 );  
}
static void my_lvgl_fs_test(void) 
{
    
	lv_fs_file_t lv_file;
  	lv_fs_res_t  lv_res;
    
  	lv_res = lv_fs_open( &lv_file, "S:/2048/2048.bin", LV_FS_MODE_RD );
  	if ( lv_res != LV_FS_RES_OK ) {
    
    	printf( "LVGL FS open error. (%d)\n", lv_res );
  	} else 
  		printf( "LVGL FS open Ok\n" );
	lv_fs_close(&lv_file);
}
 */


/*
1)BLE initialize
2)backlight setting
3)lvgl gui configuration*/
void setup()
{
  Serial.begin(115200);
	/*** Init screen ***/
	screen.init();
	//lv_png_init();
	screen.setBackLight(0.2);

  /*** Init IMU as input device ***/
  lv_port_indev_init();
 // mpu.init();
/*** Inflate GUI objects ***/
    //load_gif_3(); 
    //delay(100000);
    //load_gif_1();
    //load_gif_2();
    //setup_ui(&guider_ui);
    lv_example_gif_1();
  //load_gif_0();
  //usleep(10000);  
  //screen.init();
  //load_gif_1();
  //usleep(10000);  
  
/*tf 的初始化会影响flash内部的图片读取*/
 //tf.init();
 //lv_fs_if_init();


 // my_lvgl_fs_test();
 // my_lvgl_test();
 //String ssid = tf.readFileLine("/wifi.txt", 1);        // line-1 for WiFi ssid
// String password = tf.readFileLine("/wifi.txt", 2);    // line-2 for WiFi password
//uint8_t buf[100];
//tf.readBinFromSd("/2048/2048.bin");
 //setup_ui(&guider_ui);
    
    /*** Read WiFi info from SD-Card, then scan & connect WiFi ***/
#if 0
    wifi.init(ssid, password);

    // Change to your BiliBili UID
    Serial.println(wifi.getBilibiliFans("20259914"));
#endif


   /*there are four parameters:
   1) as the pointer to task entry function, it should be in loop(no return)
   2) name be descriptive to the task, mainly facilitate debugging
   3) the size of task stack specifized as bytes
   4) pointer to the parameter that will be used
   5) priority of the task
   6) pass back a handle by which the task can be referenced
   see up two tasks*/
    xTaskCreate(taskOne,"TaskOne",10000,NULL,1,NULL);                 
    xTaskCreate(taskTwo,"TaskTwo",10000,NULL,1,NULL);
    delay(300);
	//time1 = millis();
}

//int frame_id = 0;


void loop(){
 // run this as often as possible
    screen.routine();

    // 200 means update IMU data every 200ms
    //mpu.update(200);

 // usleep(10000);
//Serial.println("hello");
}