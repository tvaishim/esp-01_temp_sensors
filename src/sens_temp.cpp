#include <OneWire.h>
#include "rLog.h"

#include "sens_temp.h"


#define ONE_WIRE_BUS 2


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress temperatureSensors[TEMP_SENSORS_COUNT];
float temperatureValues[TEMP_SENSORS_COUNT];
uint8_t temperatureDeviceCount = 0;

void tempSensorsInit() {

  sensors.begin();

}

String strDS18B20Address(DeviceAddress deviceAddress)
{
  String str_adress = "";
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) str_adress += "0";
    str_adress += String(deviceAddress[i], HEX); 
  }
  return str_adress;
}

void findTempSensors()
{
  tempSensorsInit();
  
  temperatureDeviceCount = sensors.getDeviceCount(); // Получаем количество обнаруженных датчиков

  rlog_i("sens_find", "Find %d ds18b20 sensors", temperatureDeviceCount);

  for (uint8_t i = 0; i < temperatureDeviceCount; i++)
  {
    sensors.getAddress(temperatureSensors[i], i);
    rlog_i("sens_find", "Sensor %d: %s", i, strDS18B20Address(temperatureSensors[i]).c_str());
  }
}

void readTempSensors()
{
  sensors.requestTemperatures();
  rlog_i("sens_read", "Reading...");
  delay(1000);
  for (int i = 0; i < temperatureDeviceCount; i++)
  {
    temperatureValues[i] = sensors.getTempC(temperatureSensors[i]);
    rlog_i("sens_read", "Sensor %s value=%f", strDS18B20Address(temperatureSensors[i]).c_str(), temperatureValues[i]);
  }
}

String getStrNameSensor(uint8_t sensorNumber) {
  char output[200];
  sprintf(output, "%s/DS%s/value", settings.MQTTTopic, strDS18B20Address(temperatureSensors[sensorNumber]).c_str()); 
  return String(output);
}

String getStrValueSensor(uint8_t sensorNumber) {
  char output[20];
  sprintf(output, "%.1f", temperatureValues[sensorNumber]); 
  return String(output);
}

uint8_t getCountTempSensors() {
    return temperatureDeviceCount;
}
