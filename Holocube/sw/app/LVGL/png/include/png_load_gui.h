#ifndef LV_CUBIC_GUI_H
#define LV_CUBIC_GUI_H



#ifdef __cplusplus
extern "C" {
#endif



#include "lvgl.h"

	//extern lv_img_dsc_t screen_buffer;
    extern lv_obj_t* img;
    extern lv_obj_t* gif_bg1;
	extern lv_obj_t* gif_bg;
	extern lv_obj_t* imgbtn;
	void lv_example_gif_1(void);
    void load_gif_1(void);
    void load_gif_0(void);
	void load_gif_2(void);
	//void load_gif_3(void);
	static void my_lvgl_fs_test(void) ;
	//void load_gif_1(void);
	/* void lv_holo_cubic_gui(void);
	const lv_img_dsc_t* getN(int i);
	void showBoard(int* map);
	void showAnim(int* animMap, int direction, int newborn, int* map);
	void born(int i); */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  
