#ifndef LV_HELPER_H
#define LV_HELPER_H

#include <lvgl.h>
#include "TAMC_FT62X6.h"
#include "Wire.h"
#include <TFT_eSPI.h>

/* Display flushing */
void lh_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
/*Read the touchpad*/
void lh_touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data);

void lh_init(int rotation);
#endif // LV_HELPER_H