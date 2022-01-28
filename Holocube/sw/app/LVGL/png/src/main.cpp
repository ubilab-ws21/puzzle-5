#include <Arduino.h>
#include "display.h"
#include "lvgl.h"
#include "lv_conf.h"
#include "lv_port_indev.h"
#include "common.h"
#include <time.h>

/*** Component objects ***/
Display LCD;
Pixel rgb;

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

void setup()
{
  Serial.begin(115200);
	/*** Init screen ***/
	LCD.init();
	//lv_png_init();
	LCD.setBackLight(0.2);

  /*** Init IMU as input device ***/
  lv_port_indev_init();
/*** Inflate GUI objects ***/
    //load_gif_3(); 
    //delay(100000);
    //load_gif_1();
    //load_gif_2();
    //setup_ui(&guider_ui);
    lv_example_gif_1();
  //load_gif_0();
  //usleep(10000);  
  //LCD.init();
  //load_gif_1();
  //usleep(10000);  
  
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
    LCD_BL_PWM_CHANNEL.routine();
}