#include "lv_helper.h"
#include <WiFi.h>
#include "Wire.h"

// SDA, SCL, TP_INT, TP_RST are all defined in pin_arduino.h of dpu_esp32 variant
TAMC_FT62X6 lh_tp = TAMC_FT62X6();
Adafruit_ST7789 lh_tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

static lv_disp_draw_buf_t lh_draw_buf;
static lv_color_t lh_buf[ LH_SCREEN_WIDTH * 10 ];
static lv_disp_drv_t lh_disp_drv;
static lv_indev_drv_t lh_indev_drv;

lv_obj_t *lh_msgbox;
uint16_t width, height;

void lh_init(int rotation){
  Wire.begin(SDA, SCL);
  lh_tp.begin();
  lv_init();
  lh_tft.init(LH_SCREEN_WIDTH, LH_SCREEN_HEIGHT);
  if (rotation == 1 || rotation == 3){
    width = LH_SCREEN_HEIGHT;
    height = LH_SCREEN_WIDTH;
  } else {
    width = LH_SCREEN_WIDTH;
    height = LH_SCREEN_HEIGHT;
  }
  lh_tft.setRotation((rotation + 2) % 4);
  lh_tp.setRotation(rotation);

  lv_disp_draw_buf_init( &lh_draw_buf, lh_buf, NULL, LH_SCREEN_WIDTH * 10 );

  /*Initialize the display*/
  lv_disp_drv_init( &lh_disp_drv );
  /*Change the following line to your display resolution*/
  lh_disp_drv.hor_res = width;
  lh_disp_drv.ver_res = height;
  lh_disp_drv.flush_cb = lh_disp_flush;
  lh_disp_drv.draw_buf = &lh_draw_buf;
  lv_disp_drv_register( &lh_disp_drv );

  /*Initialize the (dummy) input device driver*/
  lv_indev_drv_init( &lh_indev_drv );
  lh_indev_drv.type = LV_INDEV_TYPE_POINTER;
  lh_indev_drv.read_cb = lh_touchpad_read;
  lv_indev_drv_register( &lh_indev_drv );
}
void lh_handler(void) {
  lv_timer_handler();
}

void lh_showMessage(const char *title, const char *message, int timeout){
  Serial.print(title);Serial.print(": ");Serial.println(message);
  lh_msgbox = lv_msgbox_create(NULL, title, message, {}, false);
  lv_obj_t *titleObj = lv_msgbox_get_title(lh_msgbox);
  lv_obj_t *msgObj = lv_msgbox_get_text(lh_msgbox);
  lv_obj_set_align(lh_msgbox, LV_ALIGN_CENTER);
  lv_obj_set_align(titleObj, LV_ALIGN_CENTER);
  lv_obj_set_align(msgObj, LV_ALIGN_CENTER);

  lv_timer_handler();
  if (timeout > 0){
    delay(timeout);
    lh_hideMessage();
  }
}
void lh_hideMessage(){
  lv_msgbox_close(lh_msgbox);
  lv_timer_handler();
}
static void lh_eventHandler(lv_event_t * event) {
  lv_event_code_t code = lv_event_get_code(event);
  lv_obj_t *obj = lv_event_get_current_target(event);
}
/* Display flushing */
void lh_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  uint32_t wh = w*h;
  lh_tft.startWrite();
  lh_tft.setAddrWindow(area->x1, area->y1, w, h);
  while (wh--) {
    lh_tft.writeColor(color_p++->full, 1);
  }
  lh_tft.endWrite();

  lv_disp_flush_ready(disp);
}
/*Read the touchpad*/
void lh_touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data) {
  lh_tp.read();

  if (!lh_tp.isTouched) {
    data->state = LV_INDEV_STATE_REL;
  }
  else{
    data->state = LV_INDEV_STATE_PR;
    /*Set the coordinates*/ 
    data->point.x = lh_tp.points[0].x;
    data->point.y = lh_tp.points[0].y;
  }
}
