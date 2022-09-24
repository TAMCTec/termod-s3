#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

#define FF20 &FreeSans24pt7b
#define FF18 &FreeSans12pt7b

#define DISPLAY_PORTRAIT 2
#define DISPLAY_LANDSCAPE 3
#define DISPLAY_PORTRAIT_FLIP 0
#define DISPLAY_LANDSCAPE_FLIP 1

static const uint8_t BAT_LV = 1;
static const uint8_t CHG = 2;

bool getChargingState() {
  return !digitalRead(CHG);
}

float getBatteryVoltage() {
  int analogVolt = analogReadMilliVolts(1);
  float voltage = analogVolt / 1000.0;
  voltage = voltage * (100.0 + 200.0) / 200.0;
  return voltage;
}

float getBatteryCapacity() {
  float voltage = getBatteryVoltage();
  float capacity = (voltage - 3.3) / (4.2 - 3.3) * 100.0;
  capacity = constrain(capacity, 0, 100);
  return capacity;
}

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(DISPLAY_LANDSCAPE);
  pinMode(CHG, INPUT_PULLUP);

  tft.fillScreen(TFT_BLACK);
  tft.drawRoundRect(40, 70, 240, 100, 10, TFT_WHITE);
  tft.drawRoundRect(270, 100, 20, 40, 4, TFT_WHITE);
  tft.fillRect(270, 100, 9, 40, TFT_BLACK); // cover the left side of the battery button
}

void loop() {
  float batteryCapacity = getBatteryCapacity();
  float batteryVoltage = getBatteryVoltage();
  bool isCharge = getChargingState();
  String batteryCapacityString = String(batteryCapacity) + String("%");
  String batteryVoltageString = String(batteryVoltage) + String("V");
  Serial.print("Battery Capacitive: ");
  Serial.println(batteryCapacity);
  Serial.print("Battery Voltage: ");
  Serial.println(batteryVoltage);
  int width = batteryCapacity * 234.0 / 100.0;

  tft.fillRoundRect(43, 73, 234, 94, 6, TFT_BLACK);
  tft.fillRoundRect(43, 73, width, 94, 6, TFT_GREEN);
  tft.setFreeFont(FF20);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE);
  tft.drawString(batteryCapacityString, 160, 120, 4);

  tft.fillRect(100, 190, 120, 20, TFT_RED);
  tft.setFreeFont(FF18);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE);
  tft.drawString(batteryVoltageString, 160, 200, 2);
  tft.fillRect(100, 210, 120, 20, TFT_RED);
  if (isCharge) {
    Serial.println("Changing");
    tft.setFreeFont(FF18);
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("Charging", 160, 220, 2);
  }

  delay(1000);
}
