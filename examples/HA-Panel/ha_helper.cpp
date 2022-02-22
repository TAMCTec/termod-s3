#include "ha_helper.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>

// SDA, SCL, TP_INT, TP_RST are all defined in pin_arduino.h of dpu_esp32 variant
TAMC_GT911 ha_tp = TAMC_GT911(SDA, SCL, TP_INT, TP_RST, HA_SCREEN_WIDTH, HA_SCREEN_HEIGHT);
TFT_eSPI ha_tft = TFT_eSPI();


static lv_disp_draw_buf_t ha_draw_buf;
static lv_color_t ha_buf[ HA_SCREEN_WIDTH * 10 ];
static lv_disp_drv_t ha_disp_drv;
static lv_indev_drv_t ha_indev_drv;
static lv_color_t ha_colorBuf;

lv_obj_t* ha_msgbox;
lv_obj_t* popupObj;

String ha_topics[12];
lv_obj_t*  ha_widgets[12];
int ha_topicIndexes[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

WiFiClient ha_wifiClient;
PubSubClient ha_mqttClient(ha_wifiClient);

void ha_init(int rotation){
  ha_tp.begin();
  lv_init();
  ha_tft.begin();

  ha_tft.setRotation(rotation);
  ha_tp.setRotation(rotation);

  lv_disp_draw_buf_init( &ha_draw_buf, ha_buf, NULL, HA_SCREEN_WIDTH * 10 );

  /*Initialize the display*/
  lv_disp_drv_init( &ha_disp_drv );
  /*Change the following line to your display resolution*/
  ha_disp_drv.hor_res = HA_SCREEN_WIDTH;
  ha_disp_drv.ver_res = HA_SCREEN_HEIGHT;
  ha_disp_drv.flush_cb = ha_disp_flush;
  ha_disp_drv.draw_buf = &ha_draw_buf;
  lv_disp_drv_register( &ha_disp_drv );

  /*Initialize the (dummy) input device driver*/
  lv_indev_drv_init( &ha_indev_drv );
  ha_indev_drv.type = LV_INDEV_TYPE_POINTER;
  ha_indev_drv.read_cb = ha_touchpad_read;
  lv_indev_drv_register( &ha_indev_drv );

  ha_mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  ha_mqttClient.setCallback(ha_mqttCallback);
}
void ha_handler(void) {
  ha_mqttClient.loop();
  lv_timer_handler();
  if (WiFi.status() != WL_CONNECTED) {
    ha_connectWifi();
  }
  if (!ha_mqttClient.connected()) {
    ha_connectMqtt();
  }
}
lv_obj_t* ha_addWidget(const char* name, int col, int row) {
  return ha_addWidget(name, col, row, 1, 1);
}
lv_obj_t* ha_addWidget(const char* name, int col, int row, int width, int height) {
  int _x, _y, _width, _height;
  _x = col * HA_WIDGET_UNIT_SIZE + HA_WIDGET_MARGIN;
  _y = row * HA_WIDGET_UNIT_SIZE + HA_WIDGET_MARGIN;
  _width = width * HA_WIDGET_UNIT_SIZE - (2 * HA_WIDGET_MARGIN);
  _height = height * HA_WIDGET_UNIT_SIZE - (2 * HA_WIDGET_MARGIN);

  lv_obj_t* widget;
  widget = lv_obj_create(lv_scr_act());
  lv_obj_set_size(widget, _width, _height);
  lv_obj_set_pos(widget, _x, _y);
  lv_obj_clear_flag(widget, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_t* label;
  label = lv_label_create(widget);
  lv_label_set_text(label, name);
  lv_obj_align(label, LV_ALIGN_TOP_LEFT, HA_LABEL_X, HA_LABEL_Y);
  lv_obj_set_size(label, HA_LABEL_WIDTH, HA_LABEL_HEIGHT);
  lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL);
  return widget;
}
lv_obj_t* ha_addToggle(const char* name, int col, int row) {
  return ha_addToggle(name, col, row, 1, 1);
}
lv_obj_t* ha_addToggle(const char* name, int col, int row, int width, int height) {
  lv_obj_t* widget = ha_addWidget(name, col, row, width, height);
  int index = row * 4 + col;
  
  lv_obj_t*  sw;
  sw = lv_switch_create(widget);
  lv_obj_set_size(sw, HA_TOGGLE_WIDTH, HA_TOGGLE_HEIGHT);
  lv_obj_align(sw, LV_ALIGN_CENTER, HA_CONTROLLER_X, HA_CONTROLLER_Y);
  lv_obj_add_event_cb(sw, ha_eventHandler, LV_EVENT_VALUE_CHANGED, &ha_topicIndexes[index]);
  lv_obj_set_ext_click_area(sw, HA_CONTROLLER_EXPAND);

  String topic = getTopic("toggle", name);
  ha_topics[index] = topic;
  ha_mqttClient.subscribe(topic.c_str());
  ha_widgets[index] = sw;

  return widget;
}
lv_obj_t* ha_addRGB(const char* name, int col, int row) {
  return ha_addRGB(name, col, row, 1, 1);
}
lv_obj_t* ha_addRGB(const char* name, int col, int row, int width, int height) {
  lv_obj_t* widget = ha_addWidget(name, col, row, width, height);
  int index = row * 4 + col;
  
  lv_obj_t* led;
  led = lv_led_create(widget);
  lv_obj_set_size(led, HA_RGB_WIDTH, HA_RGB_WIDTH);
  lv_led_on(led);
  lv_led_set_brightness(led, 255);
  lv_obj_align(led, LV_ALIGN_CENTER, HA_CONTROLLER_X, HA_CONTROLLER_Y);
  lv_obj_add_event_cb(led, ha_rgbColorWheelShow, LV_EVENT_CLICKED, &ha_topicIndexes[index]);
  lv_obj_add_event_cb(led, ha_eventHandler, LV_EVENT_VALUE_CHANGED, &ha_topicIndexes[index]);
  lv_obj_set_ext_click_area(led, HA_CONTROLLER_EXPAND);

  String topic = getTopic("rgb", name);
  ha_topics[index] = topic;
  ha_mqttClient.subscribe(topic.c_str());
  ha_widgets[index] = led;

  return widget;
}
lv_obj_t* ha_addSlider(const char* name, int col, int row, int min, int max) {
  return ha_addSlider(name, col, row, 2, 1, min, max);
}
lv_obj_t* ha_addSlider(const char* name, int col, int row, int width, int height, int min, int max) {
  lv_obj_t* widget = ha_addWidget(name, col, row, width, height);
  int index = row * 4 + col;
  
  lv_obj_t* slider;
  slider = lv_slider_create(widget);
  lv_slider_set_range(slider, min , max);
  lv_obj_set_size(slider, lv_pct(HA_SLIDER_WIDTH), HA_SLIDER_HEIGHT);
  lv_obj_align(slider, LV_ALIGN_CENTER, HA_CONTROLLER_X, HA_CONTROLLER_Y);
  lv_obj_add_event_cb(slider, ha_eventHandler, LV_EVENT_VALUE_CHANGED, &ha_topicIndexes[index]);
  lv_obj_set_ext_click_area(slider, HA_CONTROLLER_EXPAND);

  String topic = getTopic("slider", name);
  ha_topics[index] = topic;
  ha_mqttClient.subscribe(topic.c_str());
  ha_widgets[index] = slider;

  return widget;
}
lv_obj_t* ha_addValue(const char* name, int col, int row) {
  return ha_addValue(name, col, row, 1, 1);
}
lv_obj_t* ha_addValue(const char* name, int col, int row, int width, int height) {
  return ha_addValue(name, col, row, 1, 1, "");
}
lv_obj_t* ha_addValue(const char* name, int col, int row, const char* unit) {
  return ha_addValue(name, col, row, 1, 1, unit);
}
lv_obj_t* ha_addValue(const char* name, int col, int row, int width, int height, const char* unit) {
  lv_obj_t* widget = ha_addWidget(name, col, row, width, height);
  int index = row * 4 + col;
  
  lv_obj_t*  value;
  value = lv_label_create(widget);
  lv_label_set_text(value, "NAN");
  String unitString = (String)unit;
  if (unitString.length() == 0){
    lv_obj_align(value, LV_ALIGN_CENTER, HA_CONTROLLER_X, HA_CONTROLLER_Y);
  } else {
    lv_obj_align(value, LV_ALIGN_CENTER, HA_CONTROLLER_X-10, HA_CONTROLLER_Y);

    lv_obj_t*  unitObj;
    unitObj = lv_label_create(widget);
    lv_label_set_text(unitObj, unitString.c_str());
    lv_obj_align(unitObj, LV_ALIGN_CENTER, HA_CONTROLLER_X+22, HA_CONTROLLER_Y);
  }

  String topic = getTopic("value", name);
  ha_topics[index] = topic;
  ha_mqttClient.subscribe(topic.c_str());
  ha_widgets[index] = value;

  return widget;
}
lv_obj_t* ha_addTemp(const char* name, int col, int row) {
  return ha_addTemp(name, col, row, 1, 1);
}
lv_obj_t* ha_addTemp(const char* name, int col, int row, int width, int height) {
  return ha_addValue(name, col, row, width, height, "°C");
}
lv_obj_t* ha_addMeter(const char* name, int col, int row, int min, int max) {
  return ha_addMeter(name, col, row, 2, 2, min, max);
}
lv_obj_t* ha_addMeter(const char* name, int col, int row, int width, int height, int min, int max) {
  lv_obj_t* widget = ha_addWidget(name, col, row, width, height);
  int index = row * 4 + col;
  
  lv_obj_t* meter;
  meter = lv_meter_create(widget);
  lv_obj_remove_style(meter, NULL, LV_PART_MAIN);
  lv_meter_indicator_t* indic;
  lv_meter_scale_t* scale = lv_meter_add_scale(meter);
  indic = lv_meter_add_needle_line(meter, scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);
  lv_meter_set_scale_ticks(meter, scale, 41, 2, 10, lv_palette_main(LV_PALETTE_GREY));
  lv_meter_set_scale_major_ticks(meter, scale, 8, 4, 15, lv_color_black(), 10);
  lv_meter_set_scale_range(meter, scale, min, max, 270, 135);
  lv_obj_set_size(meter, lv_pct(HA_METER_WIDTH), lv_pct(HA_METER_HEIGHT));
  lv_obj_align(meter, LV_ALIGN_CENTER, HA_CONTROLLER_X, HA_CONTROLLER_Y);

  String topic = getTopic("meter", name);
  ha_topics[index] = topic;
  ha_mqttClient.subscribe(topic.c_str());
  ha_widgets[index] = meter;

  return widget;
}

String getTopic(const char* type, const char* name) {
  String topic;
  topic = (String)DEVICE_NAME_LOWER + "/" + (String)type + "/" + (String)name;
  return topic;
}
void showMessage(const char* title, const char* message, int timeout){
  Serial.print(title);Serial.print(": ");Serial.println(message);
  ha_msgbox = lv_msgbox_create(NULL, title, message, {}, false);
  lv_obj_t* titleObj = lv_msgbox_get_title(ha_msgbox);
  lv_obj_t* msgObj = lv_msgbox_get_text(ha_msgbox);
  lv_obj_set_align(ha_msgbox, LV_ALIGN_CENTER);
  lv_obj_set_align(titleObj, LV_ALIGN_CENTER);
  lv_obj_set_align(msgObj, LV_ALIGN_CENTER);

  lv_timer_handler();
  if (timeout > 0){
    delay(timeout);
    hideMessage();
  }
}
void hideMessage(){
  lv_msgbox_close(ha_msgbox);
  lv_timer_handler();
}
void ha_connectWifi() {
  showMessage("Connecting to Wi-Fi", SSID, 0);
  delay(1000);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  const char* ip = WiFi.localIP().toString().c_str();
  hideMessage();
  showMessage("Success", ip, 1000);
}
void ha_connectMqtt( ) {
  showMessage("Connecting to MQTT Server", MQTT_SERVER, 0);
  delay(1000);
  while (!ha_mqttClient.connected()) {
    if (ha_mqttClient.connect(CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {              
      String topic;
      for (int i=0; i<12; i++){
        topic = ha_topics[i];
        if (topic.length() != 0){
          Serial.print("Subscribed topic: ");Serial.println(topic.c_str());
          ha_mqttClient.subscribe(topic.c_str()); 
        }  
      }
    } else {
      delay(5000);  
    }
  }
  hideMessage();
  showMessage("Success", MQTT_SERVER, 1000);
}
void ha_sendMqtt(const char* topic, const char* value) {

}
void ha_mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived ["); 
  Serial.print(topic);
  Serial.print("] ");

  String set_str = "";
  for (int i = 0; i < length; i++) {
    set_str += (char)payload[i];
  }

  String topicType = (String)DEVICE_NAME_LOWER + "/";
  String topicString = (String)topic;
  lv_obj_t* obj;
  String t;
  for (int i=0; i<12; i++){
    t = ha_topics[i];
    if (t == topic){
      obj = ha_widgets[i];
      if (topicString.startsWith(topicType + "toggle")){
        if (set_str == "on") {
          lv_obj_add_state(obj, LV_STATE_CHECKED);
        } else if (set_str == "off") {
          lv_obj_clear_state(obj, LV_STATE_CHECKED);
        }
      } else if (topicString.startsWith(topicType + "rgb")){
        lv_led_set_color(obj, lv_color_hex(set_str.toInt()));
      } else if (topicString.startsWith(topicType + "slider")){
        lv_slider_set_value(obj, set_str.toInt(), LV_ANIM_ON);
      } else if (topicString.startsWith(topicType + "value")){
        lv_label_set_text(obj, set_str.c_str());
      } else if (topicString.startsWith(topicType + "meter")){
        lv_meter_t* meter = (lv_meter_t*)obj;
        lv_ll_t* indic_ll = &meter->indicator_ll;
        lv_meter_indicator_t* indic = (lv_meter_indicator_t*)indic_ll;
        lv_meter_set_indicator_value(obj, indic, set_str.toInt());
      }
      break;
    }
  }

  String topic_str = topic;
  Serial.println(set_str);
}

static void ha_eventHandler(lv_event_t* event) {
  lv_event_code_t code = lv_event_get_code(event);
  lv_obj_t* obj = lv_event_get_current_target(event);
  int* topicIndex = (int* )lv_event_get_user_data(event);
  int index =* topicIndex;
  const char* topic = ha_topics[index].c_str();
  String topicType = (String)DEVICE_NAME_LOWER + "/";
  String topicString = (String)topic;
  if (code == LV_EVENT_VALUE_CHANGED) {
    Serial.print("Topic: ");Serial.println(topic);
    if (topicString.startsWith(topicType + "toggle")){
      Serial.print("Value: ");Serial.println(lv_obj_has_state(obj, LV_STATE_CHECKED));
      if (lv_obj_has_state(obj, LV_STATE_CHECKED)){
        ha_mqttClient.publish(topic, "on");
      } else {
        ha_mqttClient.publish(topic, "off");
      }
    } else if (topicString.startsWith(topicType + "rgb")){
      ha_mqttClient.publish(topic, lv_label_get_text(obj));
    } else if (topicString.startsWith(topicType + "slider")){
      String valueString = (String)lv_slider_get_value(obj);
      Serial.print("Value: ");Serial.println(valueString.c_str());
      ha_mqttClient.publish(topic, valueString.c_str());
    }
  }
}
static void ha_rgbColorWheelShow(lv_event_t* event) {
  popupObj = lv_obj_create(lv_scr_act());
  lv_obj_clear_flag(popupObj, LV_OBJ_FLAG_HIDDEN);
  lv_obj_set_size(popupObj, HA_SCREEN_WIDTH, HA_SCREEN_HEIGHT);
  int* index = (int*)lv_event_get_user_data(event);
  lv_obj_t* ledObj = lv_event_get_current_target(event);

  lv_obj_t* cw;
  cw = lv_colorwheel_create(popupObj, true);
  lv_obj_align(cw, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_size(cw, 180, 180);
  lv_obj_center(cw);
  lv_obj_set_ext_click_area(cw, HA_CONTROLLER_EXPAND);
  lv_obj_add_event_cb(cw, ha_rgbColorWheelOnChange, LV_EVENT_VALUE_CHANGED, NULL);

  lv_obj_t* btn = lv_btn_create(popupObj);
  lv_obj_add_event_cb(btn, ha_rgbColorWheelOK, LV_EVENT_CLICKED, &ha_topicIndexes[*index]);
  lv_obj_align(btn, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_set_ext_click_area(btn, HA_CONTROLLER_EXPAND);
  lv_obj_t* label = lv_label_create(btn);
  lv_obj_center(label);
  lv_label_set_text(label, LV_SYMBOL_CLOSE);
}
static void ha_rgbColorWheelOnChange(lv_event_t*  event) {
  lv_obj_t* cwObj = lv_event_get_current_target(event);
  ha_colorBuf = lv_colorwheel_get_rgb(cwObj);
}
static void ha_rgbColorWheelOK(lv_event_t*  event) {
  int* index = (int*)lv_event_get_user_data(event);
  lv_obj_t* ledObj = ha_widgets[*index];
  lv_obj_add_flag(popupObj, LV_OBJ_FLAG_HIDDEN);

  lv_color_t color = ha_colorBuf;
  Serial.print("brightness: ");Serial.println(lv_color_brightness(color));
  Serial.print("r: ");Serial.println(color.ch.red);
  Serial.print("g: ");Serial.println(color.ch.green);
  Serial.print("b: ");Serial.println(color.ch.blue);
  lv_led_set_color(ledObj, color);
}
/* Display flushing */
void ha_disp_flush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p ) {
  uint32_t w = ( area->x2 - area->x1 + 1 );
  uint32_t h = ( area->y2 - area->y1 + 1 );

  ha_tft.startWrite();
  ha_tft.setAddrWindow( area->x1, area->y1, w, h );
  ha_tft.pushColors( ( uint16_t*  )&color_p->full, w*  h, true );
  ha_tft.endWrite();

  lv_disp_flush_ready( disp );
}
/*Read the touchpad*/
void ha_touchpad_read(lv_indev_drv_t* indev_driver, lv_indev_data_t* data ) {
  ha_tp.read();

  if (!ha_tp.isTouched) {
    data->state = LV_INDEV_STATE_REL;
  } else {
    data->state = LV_INDEV_STATE_PR;
    /*Set the coordinates*/ 
    data->point.x = ha_tp.points[0].x;
    data->point.y = ha_tp.points[0].y;
  }
}
