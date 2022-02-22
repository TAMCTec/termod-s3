
#ifndef TAMC_DPU_ESP32_H
#define TAMC_DPU_ESP32_H

#include "Arduino.h"

// Read Battery Voltage
float dpu_getBatteryVoltage();
// Get Battery Capacity base on voltage
float dpu_getBatteryCapacity();
// Check if is charging
bool dpu_isCharging();

#endif // TAMC_DPU_ESP32_H
