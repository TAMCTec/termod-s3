#include "Arduino.h"

float dpu_getBatteryVoltage() {
  int value = analogRead(BAT_LV);
  float voltage = value * (3.3 / 1024.0);
  // Voltage is devided by 10K and 33K resistors
  voltage = voltage * 33 / (10+33);
  return voltage;
}

float dpu_getBatteryCapacity() {
  float voltage = getBatteryVoltage();
  float capacity = map(voltage, 3.3, 4.2, 0, 100);
  return capacity;
}

bool dpu_isCharging() {
  pinMode(CHG, INPUT);
  return ~digitalRead(CHG);
}