
#ifndef LV_HELPER_H
#define LV_HELPER_H

#include <lvgl.h>
#include <TFT_eSPI.h>
#include "TAMC_GT911.h"

#define WIFI_SSID "hehehe"
#define WIFI_PASSWORD "12345678"
#define MQTT_SERVER "192.168.31.29"
#define MQTT_PORT 1883
#define MQTT_USER "homeassistant"
#define MQTT_PASSWORD "heipei5sae8Cheilooyeic6di3cheN1eithaepi6bilae3gaJe7pheequu4Voh2c"
#define CLIENT_ID "dpu-esp32-mqtt-tester"

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
