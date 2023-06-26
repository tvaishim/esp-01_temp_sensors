#pragma once

#include <DallasTemperature.h>

#include "settings.h"

#define TEMP_SENSORS_COUNT 3

extern DeviceAddress temperatureSensors[TEMP_SENSORS_COUNT];

String strDS18B20Address(DeviceAddress deviceAddress);
void tempSensorsInit();
void findTempSensors();
void readTempSensors();
String getStrNameSensor(uint8_t sensorNumber);
String getStrValueSensor(uint8_t sensorNumber);
uint8_t getCountTempSensors();
