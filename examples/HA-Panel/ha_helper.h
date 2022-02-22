
#ifndef HA_HELPER_H
#define HA_HELPER_H

#include <lvgl.h>
#include <TFT_eSPI.h>
#include "TAMC_GT911.h"
#include <WiFi.h>
#include <PubSubClient.h>

#define SSID "hehehe"
#define PASSWORD "12345678"
#define MQTT_SERVER "192.168.31.29"
#define MQTT_PORT 1883
#define MQTT_USER "homeassistant"
#define MQTT_PASSWORD "heipei5sae8Cheilooyeic6di3cheN1eithaepi6bilae3gaJe7pheequu4Voh2c"

#define DEVICE_NAME "DPU-ESP32"
#define DEVICE_NAME_LOWER "dpu-esp32"
#define CLIENT_ID "client-dpu-esp32"

#define HA_SCREEN_WIDTH  320
#define HA_SCREEN_HEIGHT 240

#define HA_WIDGET_MARGIN 2
#define HA_WIDGET_UNIT_SIZE 80
#define HA_LABEL_WIDTH 74
#define HA_LABEL_HEIGHT 30
#define HA_LABEL_X -10
#define HA_LABEL_Y -10
#define HA_TOGGLE_WIDTH 40
#define HA_TOGGLE_HEIGHT 20
#define HA_RGB_WIDTH 30
#define HA_SLIDER_WIDTH 90
#define HA_SLIDER_HEIGHT 10
#define HA_METER_WIDTH 90
#define HA_METER_HEIGHT 90
#define HA_CONTROLLER_X 0
#define HA_CONTROLLER_Y 10
#define HA_CONTROLLER_EXPAND 10

/* Display flushing */
void ha_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
/*Read the touchpad*/
void ha_touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data);

void ha_init(int rotation);
void ha_handler(void);
lv_obj_t* ha_addWidget(const char* name, int col, int row);
lv_obj_t* ha_addWidget(const char* name, int col, int row, int width, int height);
lv_obj_t* ha_addToggle(const char* name, int col, int row);
lv_obj_t* ha_addToggle(const char* name, int col, int row, int width, int height);
lv_obj_t* ha_addRGB(const char* name, int col, int row);
lv_obj_t* ha_addRGB(const char* name, int col, int row, int width, int height);
lv_obj_t* ha_addSlider(const char* name, int col, int row, int min, int max);
lv_obj_t* ha_addSlider(const char* name, int col, int row, int width, int height, int min, int max);
lv_obj_t* ha_addValue(const char* name, int col, int row);
lv_obj_t* ha_addValue(const char* name, int col, int row, int width, int height);
lv_obj_t* ha_addValue(const char* name, int col, int row, const char* unit);
lv_obj_t* ha_addValue(const char* name, int col, int row, int width, int height, const char* unit);
lv_obj_t* ha_addTemp(const char* name, int col, int row);
lv_obj_t* ha_addTemp(const char* name, int col, int row, int width, int height);
lv_obj_t* ha_addMeter(const char* name, int col, int row, int min, int max);
lv_obj_t* ha_addMeter(const char* name, int col, int row, int width, int height, int min, int max);

String getTopic(const char*, const char*);
void ha_connectWifi();
void ha_connectMqtt();
void ha_mqttCallback(char* topic, byte* payload, unsigned int length);
static void ha_eventHandler(lv_event_t* event);
static void ha_rgbColorWheelShow(lv_event_t* event);
static void ha_rgbColorWheelOnChange(lv_event_t* event);
static void ha_rgbColorWheelOK(lv_event_t* event);
void showMessage(const char* title, const char* message, int timeout);
void hideMessage();
#endif // HA_HELPER_H