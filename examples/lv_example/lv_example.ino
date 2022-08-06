#include "lvgl_helper.h"

#define DISPLAY_ROTATION 3

void setup(void) {
  Serial.begin(115200);
  Serial.println("Start");
  lh_init(DISPLAY_ROTATION);

  lv_obj_t * arc = lv_arc_create(lv_scr_act());
  lv_obj_set_size(arc, 150, 150);
  lv_arc_set_rotation(arc, 135);
  lv_arc_set_bg_angles(arc, 0, 270);
  lv_arc_set_value(arc, 40);
  lv_obj_center(arc);

  Serial.println("UI done");
}

void loop() {
  lh_handler();
}
