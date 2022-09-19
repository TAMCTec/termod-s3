#include <TFT_eSPI.h>
#include <TAMC_FT62X6.h>
#include <Wire.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

TFT_eSPI tft = TFT_eSPI();
TAMC_FT62X6 tp = TAMC_FT62X6();

#define FF18 &FreeSans12pt7b
#define GFXFF 1

// io index
uint8_t i = 0;
uint8_t lastI = -1;
// last millis
uint32_t t = 0;

uint8_t rowHeight = 30;

String touchInfo;
String batteryInfo;
String sdCardInfo;
String buttonInfo;

void setup() {
  Serial.begin(115200);
  // SD Card
  bool sdCardPresent = SD.begin(SD_CS);
  sdCardInfo += String("Mount") + (sdCardPresent ? "ed" : " Failed");

  Wire.begin();
  tft.init();
  if (!tp.begin()) {
    Serial.println("Touchscreen not found");
    while (1);
  }
  tp.setRotation(DISPLAY_LANDSCAPE);
  tft.setRotation(DISPLAY_LANDSCAPE);

  pinMode(0, INPUT_PULLUP);

  tft.setFreeFont(&FreeSans9pt7b);
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_WHITE);
  t = millis();
  tft.fillScreen(TFT_BLACK);
  uint8_t currentY = 10;
  tft.drawString("Factory test", 10, currentY, 1);
  currentY += rowHeight;
  tft.drawString("Touch:", 10, currentY, 1);
  currentY += rowHeight;
  tft.drawString("Battery:", 10, currentY, 1);
  currentY += rowHeight;
  tft.drawString("SD Card:", 10, currentY, 1);
  tft.drawString(sdCardInfo.c_str(), 110, currentY, 1);
  currentY += rowHeight;
  tft.drawString("Button IO0:", 10, currentY, 1);
  Serial.println("Hello");
}

void loop() {
  int x = 0;
  int y = 0;
  String newTouchInfo;
  String newBatteryInfo;
  String newSdCardInfo;
  String newButtonInfo;

  // Touch
  tp.read();
  if (tp.isTouched) {
    x = tp.points[0].x;
    y = tp.points[0].y;
    newTouchInfo += "[" + String(x) + ", " + String(y) + "]";
    if (tp.touches == 2){
      x = tp.points[1].x;
      y = tp.points[1].y;
      newTouchInfo += ", [" + String(x) + ", " + String(y) + "]";
    }
  } else {
    newTouchInfo += "No touch";
  }

  // Battery
  float batteryVoltage = getBatteryVoltage();
  float batteryPercentage = getBatteryCapacity();
  bool charging = getChargingState();
  newBatteryInfo += String(batteryVoltage) + "V, " + String(batteryPercentage) + "%" + (charging ? " (charging)" : "");

  // Button
  bool buttonPressed = digitalRead(0) == LOW;
  newButtonInfo += buttonPressed ? String("Pressed") : String("Released");

  uint8_t currentY = 10;
  currentY += rowHeight;
  if (newTouchInfo != touchInfo) {
    touchInfo = newTouchInfo;
    tft.fillRect(110, currentY, 210, rowHeight, TFT_BLACK);
    tft.drawString(touchInfo.c_str(), 110, currentY, 1);
  }
  currentY += rowHeight;
  if (newBatteryInfo != batteryInfo) {
    batteryInfo = newBatteryInfo;
    tft.fillRect(110, currentY, 210, rowHeight, TFT_BLACK);
    tft.drawString(batteryInfo.c_str(), 110, currentY, 1);
  }
  currentY += rowHeight;
  currentY += rowHeight;
  if (newButtonInfo != buttonInfo) {
    buttonInfo = newButtonInfo;
    tft.fillRect(110, currentY, 210, rowHeight, TFT_BLACK);
    tft.drawString(buttonInfo.c_str(), 110, currentY, 1);
  }
  delay(10);
}
