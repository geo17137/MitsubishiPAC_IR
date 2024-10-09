#ifndef MAIN_H
#define MAIN_H
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include "const.h"
#include "files.h"
#include "ir_command.h"

int dly;
FileLittleFS *fileParam;
IrCommand *irCommand;
IPAddress ipAdress;

IRMitsubishiAC *ac; // Set the GPIO used for sending messages.
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

void PubSubCallback(char *topic, byte *payload, unsigned int length);

#endif