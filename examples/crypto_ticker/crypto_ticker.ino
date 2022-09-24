#include <TFT_eSPI.h>
#include <TAMC_FT62X6.h>

#include <WiFi.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <stdlib.h>

#include "crypto_ticker.h"
#include "secret.h"

#define DISPLAY_PORTRAIT 2
#define DISPLAY_LANDSCAPE 3
#define DISPLAY_PORTRAIT_FLIP 0
#define DISPLAY_LANDSCAPE_FLIP 1

#define DISPLAY_WIDTH  240
#define DISPLAY_HEIGHT 320

// Instances
TFT_eSPI tft = TFT_eSPI();
TAMC_FT62X6 tp = TAMC_FT62X6();

// Global variables
HTTPClient http;
DynamicJsonDocument coinData(1024);
String header;

bool wifiFailed = false;

const char* coinId;
const char* coinSymbol;
const char* coinPrice;
const char* coinRank;
const char* coinChange24Hr;

String symbol = String(coinSymbol);
double price = 0;
String rank = String(coinRank);
double change24HrValue = round(String(coinChange24Hr).toFloat());

int currentCoinId = 0;
int lastCoinId = -1;

bool displayNeedReflash = false;
bool dataNeedReflash = false;

// Functions
void setStatus(int status);
void touchHandler();
bool coinGetData(String id);
void displayDrawMain(void);
void displayReflashData(void);
void displayDrawMessage(String msg);
void displayDrawMessage(String msg1, String msg2);
bool wifiInit();
String significentNumber(double f, int num);


void setup(void) {
  Serial.begin(115200);
  Serial.println("Crypto Ticker Start!");
  Wire.begin();
  tft.init();
  tp.begin();
  tp.setRotation(DISPLAY_LANDSCAPE);
  tft.setRotation(DISPLAY_LANDSCAPE);
  tft.fillScreen(TFT_BLACK);
}

int lastX = -1;
int lastY = -1;
int retryCount = 0;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
void loop() {
  if (WiFi.status() == WL_CONNECTED){
    currentMillis = millis();
    touchHandler();
    if (currentMillis - previousMillis > REFLASH_DELAY || dataNeedReflash) {
      bool success = false;
      setStatus(STATUS_BUSY);
      for (retryCount=0; retryCount<RETRY_COUNT; retryCount++){
        if (coinGetData(COINS[currentCoinId])){
          success = true;
          break;
        }
      }
      if (!success) {
        setStatus(STATUS_ERROR);
        displayDrawMessage("Get Data Error");
      } else {
        setStatus(STATUS_DONE);
        if (displayNeedReflash) {
          displayDrawMain();
          displayNeedReflash = false;
        }
        displayReflashData();
      }
      setStatus(STATUS_IDLE);
      dataNeedReflash = false;
    }
    if (currentMillis - previousMillis > LOOP_DELAY) {
      previousMillis = currentMillis;
    }
  }
  else {
    if (wifiInit()){
      Serial.println(WiFi.localIP().toString());
      String msg = String("IP: ") + WiFi.localIP().toString();
      displayDrawMessage("Connected", msg);
      delay(2000);
      displayDrawMain();
    }
  }
}

// Status
void setStatus(int status) {
  tft.fillCircle(300, 20, 10, status_colors[status]);
}

/*
 * Touch Handler
 */
void touchHandler() {
  int x = 0;
  int y = 0;
  tp.read();
  if (tp.isTouched) {
    x = tp.points[0].x;
    y = tp.points[0].y;
    if (lastX == -1) {
      if (y > 70 && y < 140) {
        if (x > 0 && x < 50) {
          currentCoinId--;
          if (currentCoinId < 0) {
            currentCoinId = COINS_LENGTH - 1;
          }
          dataNeedReflash = true;
          displayNeedReflash = true;
        } else if (x > 270 && x < 320) {
          currentCoinId++;
          if (currentCoinId >= COINS_LENGTH) {
            currentCoinId = 0;
          }
          dataNeedReflash = true;
          displayNeedReflash = true;
        }
      }
      lastX = x;
      lastY = y;
    }
  } else {
    lastX = -1;
    lastY = -1;
  }
}

/*
 * Display
 */
void displayDrawMain(void) {
  tft.fillScreen(TFT_BLACK);
  tft.setFreeFont(FF18);
  tft.setTextDatum(TR_DATUM);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Rank:", 170, 150, GFXFF);
  tft.drawString("Change 24hr:", 170, 180, 4);
  tft.fillTriangle(320 - 30, 90, 320 - 30, 120, 320 - 10, 105, TFT_PINK);
  tft.fillTriangle(      30, 90,       30, 120,       10, 105, TFT_PINK);
  for (int i=0; i<COINS_LENGTH; i++){
    if (i == currentCoinId){
      tft.fillCircle(60+(i*20), 220, 6, TFT_PINK);
    } else {
      tft.fillCircle(60+(i*20), 220, 2, TFT_WHITE);
    }
  }
}

void displayReflashData(void) {
  String id = String(coinId);

  String _symbol = String(coinSymbol);
  double _price = String(coinPrice).toFloat();
  String _rank = String(coinRank);
  double _change24HrValue = String(coinChange24Hr).toFloat();

  if (currentCoinId != lastCoinId) {
    symbol = _symbol;
    tft.fillRect(0, 0, 200, 50, TFT_BLACK);

    // Symbol
    tft.setFreeFont(FF19);
    tft.setTextDatum(TL_DATUM);
    tft.setTextColor(TFT_WHITE);
    tft.drawString(symbol, 10, 10, GFXFF);

    lastCoinId = currentCoinId;
  }

  // Price
  if (price != _price){
    if (_change24HrValue < 0){
      tft.setTextColor(TFT_RED);
    } else {
      tft.setTextColor(TFT_GREEN);
    }
    price = _price;
    String priceString = String("$") + significentNumber(price, 5);
    tft.fillRect(50, 80, 220, 50, TFT_BLACK);
    tft.setFreeFont(FF20);
    tft.setTextDatum(MC_DATUM);
    tft.drawString(priceString, 160, 100, GFXFF);
  }

  // Rank
  if (rank != _rank){
    rank = _rank;
    tft.fillRect(200, 150, 320, 20, TFT_BLACK);
    tft.setFreeFont(FF18);
    tft.setTextDatum(TL_DATUM);
    tft.setTextColor(TFT_WHITE);
    tft.drawString(rank, 200, 150, GFXFF);
  }

  // Change 24 hour
  if (change24HrValue != _change24HrValue){
    change24HrValue = _change24HrValue;

    String change24Hr = significentNumber(change24HrValue, 4) + String("%");
    tft.fillRect(200, 180, 320, 20, TFT_BLACK);
    tft.setFreeFont(FF18);
    tft.setTextDatum(TL_DATUM);
    tft.setTextColor(TFT_WHITE);
    tft.drawString(change24Hr, 200, 180, 4);
  }
}

void displayDrawMessage(String msg) {
  tft.fillRoundRect(40, 50, 240, 140, 10, TFT_BLACK);
  tft.drawRoundRect(40, 50, 240, 140, 10, TFT_CYAN);
  tft.setFreeFont(FF17);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(msg, 160, 120, GFXFF);
  displayNeedReflash = true;
}

void displayDrawMessage(String msg1, String msg2) {
  tft.fillRoundRect(40, 50, 240, 140, 10, TFT_BLACK);
  tft.drawRoundRect(40, 50, 240, 140, 10, TFT_CYAN);
  tft.setFreeFont(FF17);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(msg1, 160, 110, GFXFF);
  tft.drawString(msg2, 160, 130, GFXFF);
  displayNeedReflash = true;
}

bool coinGetData(String id) {
  Serial.print("[HTTP] begin...\n");
  http.begin("https://api.coincap.io/v2/assets/" + id); //HTTP

  Serial.print("[HTTP] GET...\n");
  int httpCode = http.GET();
  if(httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      deserializeJson(coinData, payload);
      Serial.println(payload);
      coinId = coinData["data"]["id"];
      coinSymbol = coinData["data"]["symbol"];
      coinPrice = coinData["data"]["priceUsd"];
      coinRank = coinData["data"]["rank"];
      coinChange24Hr = coinData["data"]["changePercent24Hr"];
      http.end();
      return true;
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    http.end();
    return false;
  }
}

/*
 * Wi-Fi 
 */
bool wifiInit() {
  displayDrawMessage("Connecting to", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  return wifiConnecting(WIFI_SSID);
}

bool wifiConnecting(String msg){
  int WLcount = 0;
  while (1) {
    ++WLcount;
    delay(500);
    if (WLcount > WIFI_TIMEOUT * 2) {
      displayDrawMessage("Connection Failed");
      delay(2000);
      return false;
    }
    if (WiFi.status() == WL_CONNECTED){
      return true;
    }
  }
}

String significentNumber(double f, int num){
  String result = String(f, num);
  result = result.substring(0, num+1);
  if (result.indexOf(".") < 0 || result.indexOf(".") == 5){
    result = result.substring(0, num);
  }
  return result;
}
