#include "mqtt.h"
#include "sens_temp.h"
#include "settings.h"
#include "rLog.h"

#include <ESP8266WiFi.h>
#include <PubSubClient.h>


WiFiClient wifiClient;
PubSubClient client(wifiClient);


void callback(char* p_topic, byte* p_payload, unsigned int p_length) {
}

void MQTTPublishData_() {

  WiFi.mode(WIFI_STA);
  WiFi.begin(settings.WiFissid, settings.WiFiPass);
  rlog_i("wifi", "connection...");

  findTempSensors();
  readTempSensors();

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    i++;
    if (i > 60) {
      rlog_i("wifi", "connection failed!");
      return;
    }
    delay(500);
  }
  rlog_i("wifi", "connection OK");

  for (int i = 0; i < getCountTempSensors(); i++) {

    String sTopic = getStrNameSensor(i);
    String sValue = getStrValueSensor(i);

    rlog_i("mqtt", "topic: %s, value: %s", sTopic.c_str(), sValue.c_str());

    if (client.connect(settings.MQTTClientID, settings.MQTTUser, settings.MQTTPass)) {
      rlog_i("mqtt", "sending...");
      client.publish(sTopic.c_str(), sValue.c_str(), true);
      client.loop();
      client.disconnect();
      rlog_i("mqtt", "disconnect");
    }
  }

  WiFi.disconnect();
  rlog_i("wifi", "disconnect");

}

void MQTTInit() {

  client.setServer(settings.MQTTServerAddress, settings.MQTTServerPort);
  client.setCallback(callback);

  rlog_i("mqtt_init", "MQTT initialized");
  
}




