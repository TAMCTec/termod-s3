#include "lv_helper.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>

// Set Display Rotation
// 0: Portrait, 1: Landscape, 2: Portrait Reverse, 3: Landscape Reverse
#define DISPLAY_ROTATION 3

#define REFLASH_DELAY 20 * 1000
#define COINS_LENGTH 20

lv_obj_t* coinListObj;

HTTPClient http;
DynamicJsonDocument coinList(10000);

unsigned long previousMillis = -REFLASH_DELAY; // For Reflash first time
unsigned long currentMillis = 0;
int currentCoinIndex = 0;

void connectWifi(void);
bool coinGetList();
void displayDrawList(void);
void displayUpdateList(void);
String significentNumber(double f, int num);

void setup(void) {
  Serial.begin(115200);
  Serial.println("Crypto Ticker Start!");
  Wire.begin(SDA, SCL);
  lh_init(DISPLAY_ROTATION);
  displayDrawList();
}

bool success;

void loop() {
  if (WiFi.status() != WL_CONNECTED){
    connectWifi();
  }
  currentMillis = millis();
  if (currentMillis - previousMillis > REFLASH_DELAY) {
    Serial.println(currentMillis);
    previousMillis = currentMillis;
    success = coinGetList();
    if (success){
      displayUpdateList();
    }
  }
  lh_handler();
}

/*
 * Display
 */
void displayDrawList(void) {
  coinListObj = lv_list_create(lv_scr_act());
  lv_obj_set_size(coinListObj, 320, 240);
  lv_obj_set_align(coinListObj, LV_ALIGN_CENTER);
  lv_obj_set_pos(coinListObj, 0, 0);
  for (int i=0; i<COINS_LENGTH; i++){
    lv_obj_t* obj = lv_list_add_btn(coinListObj, NULL, "");

    lv_obj_t* nameObj = lv_label_create(obj);
    lv_obj_add_flag(nameObj, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_align(nameObj, LV_ALIGN_LEFT_MID);
    lv_obj_set_pos(nameObj, 20, 0);
    lv_group_remove_obj(nameObj);
    lv_obj_t* priceObj = lv_label_create(obj);
    lv_obj_add_flag(priceObj, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_align(priceObj, LV_ALIGN_LEFT_MID);
    lv_obj_set_pos(priceObj, 140, 0);
    lv_group_remove_obj(priceObj);
    lv_obj_t* change24HrObj = lv_label_create(obj);
    lv_obj_add_flag(change24HrObj, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_align(change24HrObj, LV_ALIGN_LEFT_MID);
    lv_obj_set_pos(change24HrObj, 230, 0);
    lv_label_set_recolor(change24HrObj, true);
    lv_group_remove_obj(change24HrObj);
    // Placeholder for coin id
    lv_obj_t* idObj = lv_label_create(obj);
    lv_obj_add_flag(idObj, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_add_flag(idObj, LV_OBJ_FLAG_HIDDEN);
    lv_group_remove_obj(idObj);
  }
}
void displayUpdateList(void){
  String price, change24Hr;
  for (int i=0; i<COINS_LENGTH; i++){
    const char* rankStr = coinList["data"][i]["rank"];
    const char* nameStr = coinList["data"][i]["name"];
    const char* priceStr = coinList["data"][i]["priceUsd"];
    const char* change24HrStr = coinList["data"][i]["changePercent24Hr"];
    const char* idStr = coinList["data"][i]["id"];
    price = "$" + String(String(priceStr).toFloat(), 2);
    change24Hr = String(String(change24HrStr).toFloat(), 2) + "%";
    
    String color;
    if (change24Hr.startsWith("-")){
      color = "#ff3333"; // Red
    } else {
      color = "#33ff33"; // Green
    }
    change24Hr = color + " " + change24Hr + "#";
    Serial.print("rank: ");Serial.println(rankStr);
    Serial.print("name: ");Serial.println(nameStr);
    Serial.print("price: ");Serial.println(price.c_str());
    Serial.print("change24Hr: ");Serial.println(change24Hr.c_str());
    lv_obj_t* item = lv_obj_get_child(coinListObj, i);
    lv_label_set_text(lv_obj_get_child(item, 0), rankStr);
    lv_label_set_text(lv_obj_get_child(item, 1), nameStr);
    lv_label_set_text(lv_obj_get_child(item, 2), price.c_str());
    lv_label_set_text(lv_obj_get_child(item, 3), change24Hr.c_str());
    lv_label_set_text(lv_obj_get_child(item, 4), idStr);
  }
}

bool coinGetList() {
  String url = "https://api.coincap.io/v2/assets/?limit=";
  url += COINS_LENGTH;
  Serial.print("coinGetList url: ");Serial.println(url.c_str());
  http.begin(url.c_str()); 
  int httpCode = http.GET();
  if(httpCode > 0) {
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      deserializeJson(coinList, payload);
      Serial.println(payload);
    } else {
      String msg = "[HTTP] GET... failed, error: " + http.errorToString(httpCode);
      Serial.printf(msg.c_str());
      lh_showMessage("Error", msg.c_str(), 2000);
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      return false;
    }
  }else {
    String msg = "[HTTP] GET... failed, error: " + http.errorToString(httpCode);
    Serial.printf(msg.c_str());
    lh_showMessage("Error", msg.c_str(), 2000);
    return false;
  }
  http.end();
  return true;
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

String roundNumber(double f, int num){
  String result = String(f, num);
  result = result.substring(0, num+1);
  if (result.indexOf(".") < 0 || result.indexOf(".") == 5){
    result = result.substring(0, num);
  }
  return result;
}

String significentNumber(double f, int num){
  String result = String(f, num);
  result = result.substring(0, num+1);
  if (result.indexOf(".") < 0 || result.indexOf(".") == 5){
    result = result.substring(0, num);
  }
  return result;
}
