#include "rLog.h"
#include "settings.h"
#include "web.h"
#include "sens_temp.h"
#include "mqtt.h"


#define PIN_BUTTON 3
#define PIN_LED 1


void setup() {
  
  delay(2000);

  // #if CONFIG_RLOG_PROJECT_LEVEL >= RLOG_LEVEL_INFO
  //   // Задержка, что-бы устепь открыть монитор
  //   delay(10000);
  //   Serial.begin(9600);
  // #endif
  
  pinMode (PIN_LED, OUTPUT);
  pinMode (PIN_BUTTON, INPUT);

  rlog_i("main", "Start");

  SettingsInit();
  settings = readSettings();

  tempSensorsInit();
  findTempSensors();

  bool setting_mode = 0;

  for (uint8_t i=0; i < 50; i++) {
    if (!digitalRead(PIN_BUTTON)) {
      rlog_i("btn", "Button pressed");
      setting_mode = true;
      break;
    }
    digitalWrite(PIN_LED, !digitalRead(PIN_LED));
    delay(200);
  }

  if (setting_mode) {
    
    webSettingsStart();
    uint32_t ledt = 0;
    //Пока работает портал настроек, дальше не идем
    while (WebSettingsPortalTick()) {
      if (millis() - ledt >= 500) {
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
        ledt = millis();
      }
    }
  }

  digitalWrite(PIN_LED, 1);

  // // пытаемся подключиться
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(settings.WiFissid, settings.WiFiPass);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  // }

  // webSensorStart(webSensorPortal);
  // webSensorPortal.start();

  MQTTInit();

}

void loop() {

  rlog_i("loop", "loop");

  // webSensorPortal.tick();

  digitalWrite(PIN_LED, 0);
  MQTTPublishData_();
  digitalWrite(PIN_LED, 1);

  delay(settings.SensorPeriod * 1000);

}
