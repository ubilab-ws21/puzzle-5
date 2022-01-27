# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

FILE(GLOB_RECURSE app_sources E:/c++_tencent_class/lvgl/lvgl_png_load_01/src/'*')

idf_component_register(SRCS ${app_sources}
REQUIRES TFT_eSPI  TFT_Drivers  lvgl  lv_lib_png  MPU6050 lv_lib_gif )
target_compile_definitions(${COMPONENT_LIB} PRIVATE LV_CONF_INCLUDE_SIMPLE=1)
