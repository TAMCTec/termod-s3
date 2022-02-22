#include "ha_helper.h"

// Set Display Rotation
// 0: Portrait, 1: Landscape, 2: Portrait Reverse, 3: Landscape Reverse
#define DISPLAY_ROTATION 3

void setup() {
  Serial.begin(115200);
  ha_init(DISPLAY_ROTATION);

  lv_obj_t* button = ha_addToggle("Button", 0, 0);
  lv_obj_t* temp = ha_addValue("Temp", 1, 0, "°C");
  lv_obj_t* hum = ha_addValue("Humidity", 2, 0, "%");
  lv_obj_t* rgb = ha_addRGB("RGB", 3, 0);
  lv_obj_t* slider = ha_addSlider("Slider", 0, 1, 0, 100);
  lv_obj_t* slider2 = ha_addSlider("Slider2", 0, 2, 0, 100);
  lv_obj_t* meter = ha_addMeter("Meter", 2, 1, 0, 100);
}

void loop(){
  ha_handler();
  delay(5);
}