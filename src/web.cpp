#include "rLog.h"

#include "web.h"
#include "settings.h"
#include "sens_temp.h"

GyverPortal webSettingsPortal;
GyverPortal webSensorPortal;


void webSettingsPage() {
  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);
  GP.PAGE_TITLE("Sensor config");

  GP.FORM_BEGIN("/");

  GP_MAKE_BLOCK_TAB(
    "Wi-Fi",
    GP_MAKE_BOX(
      GP.LABEL("Login:");
      GP.TEXT("WLogn", "Login", settings.WiFissid, "", 20);
    );
    GP_MAKE_BOX(
      GP.LABEL("Pass:");
      GP.PASS("WPass", "Password", settings.WiFiPass, "", 20);
    );
  );

  GP_MAKE_BLOCK_TAB(
    "MQTT Server",
    GP_MAKE_BOX(
      GP.LABEL("Addr:");
      GP.TEXT("MAddr", "Address", settings.MQTTServerAddress, "", 30);
    );
    GP_MAKE_BOX(
      GP.LABEL("Port:");
      GP.NUMBER("MPort", "Port", settings.MQTTServerPort);
    );
    GP_MAKE_BOX(
      GP.LABEL("Login:");
      GP.TEXT("MLogn", "Login", settings.MQTTUser, "", 20);
    );
    GP_MAKE_BOX(
      GP.LABEL("Pass:");
      GP.PASS("MPass", "Password", settings.MQTTPass, "", 20);
    );
  );

  GP_MAKE_BLOCK_TAB(
    "Sensor",
    GP_MAKE_BOX(
      GP.LABEL("Client:");
    );
    GP_MAKE_BOX(
      GP.TEXT("ID", "ClientID", settings.MQTTClientID, "100%", 30);
    );
    GP_MAKE_BOX(
      GP.LABEL("Topic:");
    );
    GP_MAKE_BOX(
      GP.TEXT("Topic", "Topic", settings.MQTTTopic, "120%", 30);
    );
    GP_MAKE_BOX(
      GP.LABEL("Reading period (sec):");
      GP.NUMBER("Perd", "Period", settings.SensorPeriod);
    );
  );

  GP_MAKE_BLOCK_TAB(
    "Sensors",

    M_TABLE(
      for (uint8_t i = 0; i < getCountTempSensors(); i++) {
        M_TR(
          GP.LABEL(String("#") + i),
          GP.TEXT("", "", strDS18B20Address(temperatureSensors[i]), "", 0, "", true)
        );
      }
    );
  );

  GP.SUBMIT("OK");
  GP.FORM_END();

  GP.BUILD_END();
}

void webSettingsAction() {
  if (webSettingsPortal.form("/")) {

    webSettingsPortal.copyStr("WLogn", settings.WiFissid);
    webSettingsPortal.copyStr("WPass", settings.WiFiPass);
    webSettingsPortal.copyStr("MAddr", settings.MQTTServerAddress);
    webSettingsPortal.copyInt("MPort", settings.MQTTServerPort);
    webSettingsPortal.copyStr("MLogn", settings.MQTTUser);
    webSettingsPortal.copyStr("MPass", settings.MQTTPass);
    webSettingsPortal.copyStr("ID", settings.MQTTClientID);
    webSettingsPortal.copyStr("Topic", settings.MQTTTopic);
    webSettingsPortal.copyInt("Perd", settings.SensorPeriod);

    WiFi.softAPdisconnect();

    rlog_i("web", "Wi-Fi AP stoped");

    writeSettings(settings);

    webSettingsPortal.stop();

    rlog_i("web", "Settings portal stoped");

    rlog_i("web", "Settings server stop");
  }
}

bool WebSettingsPortalTick() {
  return webSettingsPortal.tick();
}

void webSettingsStart() {

  rlog_i("web", "Settings server start");

  WiFi.mode(WIFI_AP);
  WiFi.softAP("Sensor config");

  rlog_i("web", "Wi-Fi AP started");

  webSettingsPortal.attachBuild(webSettingsPage);
  webSettingsPortal.attach(webSettingsAction);
  webSettingsPortal.enableAuth("sensor", "sensor");
  webSettingsPortal.start();

  rlog_i("web", "Settings portal started");

}

void webSensorPage() {
  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);
  GP.PAGE_TITLE("Sensor");
  // GP.UPDATE("tt1, tt2, tt3");
  GP.UPDATE("tt3");

  GP_MAKE_BLOCK_TAB(
    "Sensor",
    GP_MAKE_BOX(
      GP.LABEL("ID:");
      GP.TEXT("", "", settings.MQTTClientID, "", 0, "", true);
    );
    GP_MAKE_BOX(
      GP.LABEL("Topic:");
      GP.TEXT("", "", settings.MQTTTopic, "", 0, "", true);
    );
    GP_MAKE_BOX(
      GP.LABEL("Value:");
      GP.TEXT("tValue", "", "", "", 0, "", true);
    );
  );

  GP.BUILD_END();
}

void webSensorAction(GyverPortal& portal) {

  char value[20];

  if (portal.update()) {

    sprintf(value, "%f", (float) random(100) / 3);

    if (portal.update("tValue")) {
      portal.answer(value);
    }

  }
}

void webSensorStart(GyverPortal& portal) {

  // запускаем портал
  portal.attachBuild(webSensorPage);
  portal.attach(webSensorAction);
  // webSensorPortal.start();
 
}
