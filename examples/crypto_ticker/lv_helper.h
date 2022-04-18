
#ifndef LV_HELPER_H
#define LV_HELPER_H

#include <lvgl.h>
#include "TAMC_FT62X6.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#define TFT_MOSI 11
#define TFT_SCLK 12
#define TFT_CS   10
#define TFT_RST  14
#define TFT_DC    9

#define LH_SCREEN_WIDTH  240
#define LH_SCREEN_HEIGHT 320


/* Display flushing */
void lh_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
/*Read the touchpad*/
void lh_touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data);

void lh_init(int rotation);
void lh_handler(void);
void lh_showMessage(const char *title, const char *message, int timeout);
void lh_hideMessage();
static void lh_eventHandler(lv_event_t * event);
#endif // LV_HELPER_H
