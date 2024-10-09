#ifndef CONST_H
#define CONST_H
#include "../secret/password.h"

#define version "2024.K09.20"

const char *mqttServer = MQTT_SERVER;
const int  mqttPort = MQTT_PORT;
const char *mqttUser = MQTT_USER;
const char *mqttPassword = MQTT_PASSWORD;
const char *ssid = SSID;
const char *password = PASSWORD;
const char *hostname = HOSTNAME;

// Paramètre numéro 
// 0 : 1 on, 0 off
// 1 : température par défaut
// 2 : fan mode voir ci-dessous
// 3 : mode de fonctionnement défaut HEAT
// 4 : position de la vanne défaut kMitsubishiAcVaneHighest
const char *param = "0:17:1:1:1";

#ifdef ESP_01
#define IR_LED 2
#else
// esp01-m
#define IR_LED 4
#endif

const uint16_t kIrLed = IR_LED; // ESP8266 GPIO pin to use. Recommended: 4 (D2).

// IR_COMMAND
#define HEAT kMitsubishiAcHeat
#define COOL kMitsubishiAcCool
#define AUTO kMitsubishiAcAuto

#define FAN_AUTO kMitsubishiAcFanAuto
#define FAN_SILENT kMitsubishiAcFanSilent
#define FAN_REAL_MAX kMitsubishiAcFanRealMax
#define FAN_MAX  kMitsubishiAcFanRealMax
#define FAN_1 1
#define FAN_2 2
#define FAN_3 3
#define FAN_4 4
#define FAN_5 5

// Nota inversion probable des valeur kMitsubishiAcVaneHighest correspond à kMitsubishiAcVaneLowest
#define VANE_AUTO kMitsubishiAcVaneAuto
#define VANE_HIGHEST kMitsubishiAcVaneHighest
#define VANE_HIGH kMitsubishiAcVaneHigh
#define VANE_MIDDLE kMitsubishiAcVaneMiddle
#define VANE_LOW kMitsubishiAcVaneLow
#define VANE_LOWEST kMitsubishiAcVaneLowest
#define VANE_SWING kMitsubishiAcVaneSwing

// Abonnement
#define TOPIC_PAC_IR_PARAM_SET  "mitsubishi/param/set"
#define TOPIC_PAC_IR_PARAM_GET  "mitsubishi/param/get"
#define TOPIC_PAC_IR_PARAM_APPLY "mitsubishi/param/apply"

#define TOPIC_PAC_IR_ON         "mitsubishi/param/on"
#define TOPIC_PAC_IR_OFF        "mitsubishi/param/off"
#define TOPIC_PAC_IR_TEMP       "mitsubishi/param/temp"
#define TOPIC_PAC_IR_MODE       "mitsubishi/param/mode"
#define TOPIC_PAC_IR_FAN        "mitsubishi/param/fan"
#define TOPIC_PAC_IR_VANNE      "mitsubishi/param/vanne"

#define TOPIC_PAC_IR_VERSION_GET    "mitsubishi/get_version"

// Publication
#define TOPIC_PAC_IR_PARAM_PUB "mitsubishi/param/pub"
#define TOPIC_PAC_IR_VERSION "mitsubishi/version"

#endif