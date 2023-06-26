#include <EEPROM.h>
#include "rLog.h"

#include "settings.h"


#define EEPROM_BUFFER_LENGTH 512
#define EEPROM_SETTINGS_ADDR 0
#define CRC_STR "OK"


Settings settings;


void SettingsInit() {
  EEPROM.begin(EEPROM_BUFFER_LENGTH);
  rlog_i("eeprom", "Size: %d", EEPROM.length());
}

Settings newSettings() {
  Settings settings;

  strcpy(settings.WiFissid, "");
  strcpy(settings.WiFiPass, "");
  strcpy(settings.MQTTServerAddress, "");
  settings.MQTTServerPort = 1883;
  strcpy(settings.MQTTUser, "");
  strcpy(settings.MQTTPass, "");
  strcpy(settings.MQTTClientID, "TEMP_SENSOR_001");
  strcpy(settings.MQTTTopic, "tempsensor001");
  settings.SensorPeriod = 30;
  strcpy(settings.crc, CRC_STR);

  rlog_i("new_settings", "New settings");

  return settings;
}

Settings readSettings() {
  Settings settings;
  
  rlog_i("settings", "Reading settings...");
  
  EEPROM.get(EEPROM_SETTINGS_ADDR, settings);
  
  if (strcmp(settings.crc, CRC_STR) != 0) {
    rlog_i("settings", "Settings failed");
    settings = newSettings();
  }

  rlog_i("settings", "WiFissid: %s", settings.WiFissid);
  rlog_i("settings", "WiFiPass: %s", settings.WiFiPass);
  rlog_i("settings", "MQTTServerAddress: %s", settings.MQTTServerAddress);
  rlog_i("settings", "MQTTServerPort: %d", settings.MQTTServerPort);
  rlog_i("settings", "MQTTUser: %s", settings.MQTTUser);
  rlog_i("settings", "MQTTPass: %s", settings.MQTTPass);
  rlog_i("settings", "MQTTClientID: %s", settings.MQTTClientID);
  rlog_i("settings", "MQTTTopic: %s", settings.MQTTTopic);
  rlog_i("settings", "SensorPeriod: %d", settings.SensorPeriod);
  rlog_i("settings", "crc: %s", settings.crc);

  return settings;
}

bool SettingsIsChanged(Settings s1, Settings s2) {
  return (
    (strcmp(s1.WiFissid, s2.WiFissid) != 0) or
    (strcmp(s1.WiFiPass, s2.WiFiPass) != 0) or
    (strcmp(s1.MQTTServerAddress, s2.MQTTServerAddress) != 0) or
    (s1.MQTTServerPort != s2.MQTTServerPort) or
    (strcmp(s1.MQTTUser, s2.MQTTUser) != 0) or
    (strcmp(s1.MQTTPass, s2.MQTTPass) != 0) or
    (strcmp(s1.MQTTClientID, s2.MQTTClientID) != 0) or
    (strcmp(s1.MQTTTopic, s2.MQTTTopic) != 0) or
    (s1.SensorPeriod != s2.SensorPeriod)
  );
}

void writeSettings(Settings settings) {
  Settings oldSettings;
  oldSettings = readSettings();
  if (SettingsIsChanged(settings, oldSettings)) {

    rlog_i("settings", "Writing settings...");

    EEPROM.put(EEPROM_SETTINGS_ADDR, settings);
    EEPROM.commit();

    rlog_i("settings", "WiFissid: %s", settings.WiFissid);
    rlog_i("settings", "WiFiPass: %s", settings.WiFiPass);
    rlog_i("settings", "MQTTServerAddress: %s", settings.MQTTServerAddress);
    rlog_i("settings", "MQTTServerPort: %d", settings.MQTTServerPort);
    rlog_i("settings", "MQTTUser: %s", settings.MQTTUser);
    rlog_i("settings", "MQTTPass: %s", settings.MQTTPass);
    rlog_i("settings", "MQTTClientID: %s", settings.MQTTClientID);
    rlog_i("settings", "MQTTTopic: %s", settings.MQTTTopic);
    rlog_i("settings", "SensorPeriod: %d", settings.SensorPeriod);
  }
}

