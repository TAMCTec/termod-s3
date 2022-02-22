#include "lv_helper.h"
#include <WiFi.h>
#include <PubSubClient.h>

// Set Display Rotation
// 0: Portrait, 1: Landscape, 2: Portrait Reverse, 3: Landscape Reverse
#define DISPLAY_ROTATION 0

lv_obj_t* topic_ta;
lv_obj_t* payload_ta;
lv_obj_t* messages_obj;
lv_obj_t* messages_label;
// lv_obj_t* sub_switch;
lv_obj_t* sub_button;
lv_obj_t* pub_button;
lv_obj_t* kb;

String messages;
static const char* kb_text_lower_map[] = {
  "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "\n",
  "a","s", "d", "f", "g", "h", "j", "k", "l", "\n",
  "ABC", "z", "x", "c", "v", "b", "n", "m", LV_SYMBOL_BACKSPACE, "\n",
  "1#", LV_SYMBOL_LEFT, ",", " ", ".", LV_SYMBOL_RIGHT, LV_SYMBOL_OK, ""
};
static const char* kb_text_upper_map[] = {
  "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "\n",
  "A","S", "D", "F", "G", "H", "J", "K", "L", "\n",
  "abc", "Z", "X", "C", "V", "B", "N", "M", LV_SYMBOL_BACKSPACE, "\n",
  "1#", LV_SYMBOL_LEFT, ",", " ", ".", LV_SYMBOL_RIGHT, LV_SYMBOL_OK, ""
};
static const char* kb_special_map[] = {
  "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "\n",
  "@", "#", "$", "_", "&", "-", "+", "(", ")", "/", "\n",
  "*", "\"", "'", ":", ";", "!", "?", "{", "}", LV_SYMBOL_BACKSPACE, "\n",
  "abc", LV_SYMBOL_LEFT, "\\", " ", "=", "^", LV_SYMBOL_RIGHT, LV_SYMBOL_OK, ""
};
#define SMALL_KEY_CONTROL 16
#define BIG_KEY_CONTROL 80
#define SPACE_KEY_CONTROL 80
static const lv_btnmatrix_ctrl_t kb_text_control[] = {
  SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,
  SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,
  BIG_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,BIG_KEY_CONTROL,
  BIG_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SPACE_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,BIG_KEY_CONTROL,
};

static const lv_btnmatrix_ctrl_t kb_special_control[] = {
  SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,
  SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,
  SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,BIG_KEY_CONTROL,
  BIG_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SPACE_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,SMALL_KEY_CONTROL,BIG_KEY_CONTROL,
};

static void ta_event_cb(lv_event_t * e);
static void sub_event_cb(lv_event_t * e);
static void pub_event_cb(lv_event_t * e);
void draw_main();
void connectWifi();
void connectMqtt();
void mqttCallback(char* topic, byte* payload, unsigned int length);

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup(void) {
  Serial.begin(115200);
  Serial.println("MQTT Tester Start!");
  lh_init(DISPLAY_ROTATION);
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);
  draw_main();
}

void loop() {
  // if (WiFi.status() != WL_CONNECTED){
  //   connectWifi();
  // }
  // if (!mqttClient.connected()) {
  //   connectMqtt();
  // }
  // currentMillis = millis();
  // if (currentMillis - previousMillis > REFLASH_DELAY) {
  //   Serial.println(currentMillis);
  //   previousMillis = currentMillis;
  //   success = coinGetList();
  //   if (success){
  //     displayUpdateList();
  //   }
  // }
  lh_handler();
  // mqttClient.loop();
}

void draw_main() {
  topic_ta = lv_textarea_create(lv_scr_act());
  lv_textarea_set_one_line(topic_ta, true);
  lv_textarea_set_cursor_click_pos(topic_ta, true);
  lv_obj_set_width(topic_ta, 180);
  lv_textarea_set_placeholder_text(topic_ta, "Topic");
  lv_obj_set_align(topic_ta, LV_ALIGN_TOP_LEFT);
  lv_obj_set_pos(topic_ta, 0, 0);
  lv_obj_add_event_cb(topic_ta, ta_event_cb, LV_EVENT_ALL, NULL);
  sub_button = lv_btn_create(lv_scr_act());
  lv_obj_set_width(sub_button, 60);
  lv_obj_set_align(sub_button, LV_ALIGN_TOP_RIGHT);
  lv_obj_set_pos(sub_button, 0, 4);
  lv_obj_add_flag(sub_button, LV_OBJ_FLAG_CHECKABLE);
  lv_obj_add_event_cb(sub_button, sub_event_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_t* sub_label = lv_label_create(sub_button);
  lv_label_set_text(sub_label, "Sub");
  lv_obj_center(sub_label);
  // sub_switch = lv_switch_create(lv_scr_act());
  // lv_obj_set_width(sub_switch, 60);
  // lv_obj_set_align(sub_switch, LV_ALIGN_TOP_RIGHT);
  // lv_obj_set_pos(sub_switch, 0, 4);
  // lv_obj_add_event_cb(sub_switch, sub_event_cb, LV_EVENT_CLICKED, NULL);

  messages_obj = lv_obj_create(lv_scr_act());
  messages_label = lv_label_create(messages_obj);
  lv_obj_set_size(messages_obj, lv_pct(100), 246);
  lv_obj_set_align(messages_obj, LV_ALIGN_TOP_MID);
  lv_obj_set_pos(messages_obj, 0, 37);
  lv_label_set_text(messages_label, "");

  payload_ta = lv_textarea_create(lv_scr_act());
  lv_textarea_set_one_line(payload_ta, true);
  lv_textarea_set_cursor_click_pos(payload_ta, true);
  lv_obj_set_width(payload_ta, 180);
  lv_textarea_set_placeholder_text(payload_ta, "Payload");
  lv_obj_set_align(payload_ta, LV_ALIGN_TOP_LEFT);
  lv_obj_set_pos(payload_ta, 0, 284);
  lv_obj_add_event_cb(payload_ta, ta_event_cb, LV_EVENT_ALL, NULL);
  pub_button = lv_btn_create(lv_scr_act());
  lv_obj_set_width(pub_button, 60);
  lv_obj_set_align(pub_button, LV_ALIGN_TOP_RIGHT);
  lv_obj_set_pos(pub_button, 0, 286);
  lv_obj_add_event_cb(pub_button, pub_event_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_t* pub_label = lv_label_create(pub_button);
  lv_label_set_text(pub_label, "Pub");
  lv_obj_center(pub_label);

  kb = lv_keyboard_create(lv_scr_act());
  lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
  lv_obj_set_size(kb, LH_SCREEN_WIDTH, 140);
  lv_obj_set_align(kb, LV_ALIGN_BOTTOM_MID);
  lv_obj_set_pos(kb, 0, 0);
  lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_TEXT_LOWER, kb_text_lower_map, kb_text_control);
  lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_TEXT_UPPER, kb_text_upper_map, kb_text_control);
  lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_SPECIAL, kb_special_map, kb_special_control);
}

// To auto change keyboard input and auto resize
static void ta_event_cb(lv_event_t * e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t* ta = lv_event_get_target(e);
  switch (code) {
    case LV_EVENT_FOCUSED:
      lv_keyboard_set_textarea(kb, ta);
      lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
      lv_obj_set_size(messages_obj, lv_pct(100), 106);
      lv_obj_set_pos(payload_ta, 0, 144);
      lv_obj_set_pos(pub_button, 0, 146);
      break;
    case LV_EVENT_DEFOCUSED:
      lv_keyboard_set_textarea(kb, NULL);
      lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
      lv_obj_set_size(messages_obj, lv_pct(100), 246);
      lv_obj_set_pos(payload_ta, 0, 284);
      lv_obj_set_pos(pub_button, 0, 286);
      break;
    default:
      break;
  }
}

static void sub_event_cb(lv_event_t * e) {
  lv_obj_t* btn = lv_event_get_target(e);
  const char* topic = lv_textarea_get_text(topic_ta);
  if (lv_obj_has_state(btn, LV_STATE_CHECKED)){
    mqttClient.subscribe(topic);
    lh_showMessage("Subscribed", topic, 2);
  } else {
    mqttClient.unsubscribe(topic);
    lh_showMessage("Unsubscribed", topic, 2);
  }
}

static void pub_event_cb(lv_event_t * e) {
  lv_obj_t* btn = lv_event_get_target(e);
  const char* topic = lv_textarea_get_text(topic_ta);
  const char* payload = lv_textarea_get_text(payload_ta);
  mqttClient.publish(topic, payload);
  lh_showMessage("Published", payload, 1);
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived ["); 
  Serial.print(topic);
  Serial.print("] ");

  String set_str = "";
  for (int i = 0; i < length; i++) {
    set_str += (char)payload[i];
  }
  String topic_str = topic;
  Serial.println(set_str);
  messages += set_str += "\n";
  lv_label_set_text(messages_label, messages.c_str());
}

void connectWifi() {
  lh_showMessage("Connecting to Wi-Fi", WIFI_SSID, 0);
  delay(1000);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  const char* ip = WiFi.localIP().toString().c_str();
  lh_hideMessage();
  lh_showMessage("Success", ip, 1000);
}

void connectMqtt() {
  lh_showMessage("Connecting to MQTT Server", MQTT_SERVER, 0);
  delay(1000);
  while (!mqttClient.connected()) {
    if (mqttClient.connect(CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {              
      // String topic;
      // for (int i=0; i<12; i++){
      //   topic = topics[i];
      //   if (topic.length() != 0){
      //     Serial.print("Subscribed topic: ");Serial.println(topic.c_str());
      //     mqttClient.subscribe(topic.c_str()); 
      //   }  
      // }
    } else {
      delay(5000);  
    }
  }
  lh_hideMessage();
  lh_showMessage("Success", MQTT_SERVER, 1000);
}