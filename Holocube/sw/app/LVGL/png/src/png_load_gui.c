/*********************
 *      INCLUDES
 *********************/
#include "lv_cubic_gui.h"
#include "jiao.h"
#include "example_gif.h"
#include "p1fnr9p2sj1nqc1id51nbv1jug1v374_1.h"
#include "tit.h"
#include "common.h"
#include <time.h>
#include "number_game.h"
#include "lv_gif.h"
//#include <hint_for_puzzle.h>

void lv_example_gif_1(void)
{
    LV_IMG_DECLARE(number_game);
    lv_obj_t * img;

    img = lv_gif_create(lv_scr_act());
    lv_gif_set_src(img, &number_game);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
   /*
    img = lv_gif_create(lv_scr_act());
    lv_gif_set_src(img, "S:/example.bin");
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    */
}

/* void load_gif_0()
{
static lv_style_t default_style;
	lv_style_init(&default_style);
	lv_style_set_bg_color(&default_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_bg_color(&default_style, LV_STATE_PRESSED, LV_COLOR_GRAY);
	lv_style_set_bg_color(&default_style, LV_STATE_FOCUSED, LV_COLOR_BLACK);
	lv_style_set_bg_color(&default_style, LV_STATE_FOCUSED | LV_STATE_PRESSED, lv_color_hex(0xf88));
LV_IMG_DECLARE(jiao);
lv_obj_t * gif_bg1 = lv_img_create(lv_scr_act(),NULL);// delete the app_window
lv_img_set_src(gif_bg1,&jiao);
lv_obj_align(gif_bg1, NULL, LV_ALIGN_CENTER, 0, 0); 
//printf("hello_world");
}



void load_gif_2()
{

	lv_obj_t * imgbtn = lv_imgbtn_create( lv_scr_act(), NULL );
	// 设置imgbtn在被按下时显示所调用的图片 
  	lv_imgbtn_set_src( imgbtn, LV_BTN_STATE_PRESSED, "S:/2048/2048.bin" );
  	//设置imgbtn在被释放时显示所调用的图片 
  	lv_imgbtn_set_src( imgbtn, LV_BTN_STATE_RELEASED, "S:/2048/2048.bin" );
  	// 居中对齐 
  	lv_obj_align( imgbtn, NULL, LV_ALIGN_CENTER, 0, 0 );
}

void load_gif_1()
{
LV_IMG_DECLARE(p1fnr9p2sj1nqc1id51nbv1jug1v374_1);
lv_obj_t * gif_bg = lv_img_create(lv_scr_act(),NULL);// delete the app_window
lv_img_set_src(gif_bg,&p1fnr9p2sj1nqc1id51nbv1jug1v374_1);
lv_obj_align(gif_bg, NULL, LV_ALIGN_CENTER, 0, 0); 
//Serial.printf("hello_world");
} */
/* void load_gif_3(void)
{
    //LV_IMG_DECLARE(img_bulb_gif);
    lv_obj_t * img = lv_gif_create_from_file(lv_scr_act(),"S:2048.gif");

    // img = lv_gif_create(lv_scr_act());
    lv_gif_set_src(img, &img_bulb_gif);
    lv_obj_align(img, LV_ALIGN_LEFT_MID, 20, 0);
    // Assuming a File system is attached to letter 'A'
     * E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h 
    lv_gif_set_src(img, "S:2048.gif");
    lv_obj_align(img,NULL, LV_ALIGN_CENTER, 0, 0);
} */

