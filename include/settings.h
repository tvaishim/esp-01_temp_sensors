#pragma once

#include <stdint.h>


struct Settings {
  char WiFissid[21];
  char WiFiPass[21];
  char MQTTServerAddress[31];
  uint16_t MQTTServerPort;
  char MQTTUser[21];
  char MQTTPass[21];
  char MQTTClientID[31];
  char MQTTTopic[91];
  uint16_t SensorPeriod;
  char crc[3];
};


extern Settings settings;

void SettingsInit();
Settings readSettings();
void writeSettings(Settings settings);
