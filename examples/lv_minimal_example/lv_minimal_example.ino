#include "lv_helper.h"

void setup() {
  Serial.begin(115200);
  lh_init(DISPLAY_LANDSCAPE);
  Serial.println("LVGL Example: Ready");

  lv_obj_t* slider = lv_slider_create(lv_scr_act());
  lv_obj_align(slider, LV_ALIGN_CENTER, 0, 0);
}

void loop() {
  lv_timer_handler();
}