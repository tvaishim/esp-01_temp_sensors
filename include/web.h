#pragma once

#include <GyverPortal.h>

extern GyverPortal webSettingsPortal;
extern GyverPortal webSensorPortal;


void webSettingsStart();
void webSensorStart(GyverPortal& portal);
bool WebSettingsPortalTick();
