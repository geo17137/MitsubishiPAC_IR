#include <Arduino.h>

/* Copyright 2017, 2018 David Conran
 *
 * An IR LED circuit *MUST* be connected to the ESP8266 on a pin
 * as specified by kIrLed below.
 *
 * TL;DR: The IR LED needs to be driven by a transistor for a good result.
 *
 * Suggested circuit:
 *     https://github.com/crankyoldgit/IRremoteESP8266/wiki#ir-sending
 *
 * Common mistakes & tips:
 *   * Don't just connect the IR LED directly to the pin, it won't
 *     have enough current to drive the IR LED effectively.
 *   * Make sure you have the IR LED polarity correct.
 *     See: https://learn.sparkfun.com/tutorials/polarity/diode-and-led-polarity
 *   * Typical digital camera/phones can be used to see if the IR LED is flashed.
 *     Replace the IR LED with a normal LED if you don't have a digital camera
 *     when debugging.
 *   * Avoid using the following pins unless you really know what you are doing:
 *     * Pin 0/D3: Can interfere with the boot/program mode & support circuits.
 *     * Pin 1/TX/TXD0: Any serial transmissions from the ESP8266 will interfere.
 *     * Pin 3/RX/RXD0: Any serial transmissions to the ESP8266 will interfere.
 *   * ESP-01 modules are tricky. We suggest you use a module with more GPIOs
 *     for your first time. e.g. ESP-12 etc.
 */
/*
 * geo17137
 * Adaptation pour la commande PAC Mitsubishi et intégation dans le FrontEnd Android
*/
#include "main.h"

inline void initOTA() {
  ArduinoOTA.setHostname(hostname);
  ArduinoOTA.begin();
}

void initWifiStation() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("Wifi %s not connected! Rebooting...", ssid);
    delay(5000);
    ESP.restart();
  }
  WiFi.setHostname(hostname);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  Serial.print("IP address: ");
  ipAdress = WiFi.localIP();
  Serial.println(ipAdress);
  initOTA();
}

void onWifiConnect(const WiFiEventStationModeGotIP &event) {
  // Serial.println("Connected to Wi-Fi sucessfully.");
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected &event) {
  // Serial.println("Disconnected from Wi-Fi, trying to connect...");
  WiFi.disconnect();
  WiFi.begin(ssid, password);
}

void initMQTTClient() {
  // Connecting to MQTT server
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(PubSubCallback);
  while (!mqttClient.connected()) {
    Serial.println(String("Connecting to MQTT (") + mqttServer + ")...");
    // Pour un même courtier les clients doivent avoir un id différent
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (mqttClient.connect(clientId.c_str(), mqttUser, mqttPassword)) {
      Serial.println("MQTT client connected");
      // digitalWrite(GPIO2_LED, LOW);
    }
    else {
      Serial.print("\nFailed with state ");
      Serial.println(mqttClient.state());
      // digitalWrite(GPIO2_LED, HIGH);
      if (WiFi.status() != WL_CONNECTED) {
        initWifiStation();
      }
      delay(500);
    }
  }
  // Déclare Pub/Sub topics
  mqttClient.subscribe(TOPIC_PAC_IR_PARAM_SET);
  mqttClient.subscribe(TOPIC_PAC_IR_PARAM_GET);
  mqttClient.subscribe(TOPIC_PAC_IR_PARAM_APPLY);
  mqttClient.subscribe(TOPIC_PAC_IR_ON);
  mqttClient.subscribe(TOPIC_PAC_IR_OFF);
  mqttClient.subscribe(TOPIC_PAC_IR_TEMP);
  mqttClient.subscribe(TOPIC_PAC_IR_MODE);
  mqttClient.subscribe(TOPIC_PAC_IR_FAN);
  mqttClient.subscribe(TOPIC_PAC_IR_VANNE);
  mqttClient.subscribe(TOPIC_PAC_IR_VERSION_GET);
}

FileLittleFS *initFileParam() {
  fileParam = new FileLittleFS("param.txt");
  if (!fileParam->exist()) {
    fileParam->writeFile(param, "w");
    fileParam->close();
  }
  return fileParam;
}


void setup() {
  initWifiStation();
  initMQTTClient();
  fileParam = initFileParam();
  irCommand = new IrCommand(kIrLed);
  delay(200);
  String param = fileParam->readFile();
  fileParam->close();
  irCommand->setParam(param);
  irCommand->apply();

  // irCommand->off();
  // irCommand->setMode(COOL);
  // irCommand->setTemp(17);
  // irCommand->setFan(1);
  // irCommand->setParam(fileParam->readFile());
}

void loop() {
  ArduinoOTA.handle();
  if (!mqttClient.connected()) {
    // digitalWrite(GPIO2_LED, HIGH);
    initMQTTClient();
  }
  mqttClient.loop();
}

void PubSubCallback(char *topic, byte *payload, unsigned int length) {
  String strPayload = "";
  // Serial.print("\nTopic:");
  // Serial.print(topic);
  // Serial.print(", Message:");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    strPayload += (char)payload[i];
  }
  //------------------ TOPIC_PAC_IR_PARAM_SET ----------------------
  if (strcmp(topic, TOPIC_PAC_IR_PARAM_SET) == 0) {
    irCommand->setParam(strPayload);
    fileParam->writeFile(strPayload, "w");
    return;
  }
  //------------------ TOPIC_PAC_IR_PARAM_GET ----------------------
  if (strcmp(topic, TOPIC_PAC_IR_PARAM_GET) == 0) {
    mqttClient.publish(TOPIC_PAC_IR_PARAM_PUB, irCommand->paramGet().c_str());
    return;
  }
  //------------------ TOPIC_PAC_IR_ON ----------------------
  if (strcmp(topic, TOPIC_PAC_IR_ON) == 0) {
    irCommand->on();
    return;
  }
  //------------------ TOPIC_PAC_IR_OFF ----------------------
  if (strcmp(topic, TOPIC_PAC_IR_OFF) == 0) {
    irCommand->off();
    return;
  }
  //------------------ TOPIC_PAC_IR_TEMP ----------------------
  if (strcmp(topic, TOPIC_PAC_IR_TEMP) == 0) {
    irCommand->setTemp(atoi(strPayload.c_str()));
    return;
  }
  //------------------ TOPIC_PAC_IR_MODE ----------------------
  if (strcmp(topic, TOPIC_PAC_IR_MODE) == 0) {
    irCommand->setMode(atoi(strPayload.c_str()));
    return;
  }
  //------------------ TOPIC_PAC_IR_FAN ----------------------
  if (strcmp(topic, TOPIC_PAC_IR_FAN) == 0) {
    irCommand->setFan(atoi(strPayload.c_str()));
    return;
  }
  //------------------ TOPIC_PAC_IR_VANNE ----------------------
  if (strcmp(topic, TOPIC_PAC_IR_VANNE) == 0) {
    irCommand->setVanne(atoi(strPayload.c_str()));
    return;
  }
  //------------------ TOPIC_PAC_IR_PARAM_APPLY ----------------------
  if (strcmp(topic, TOPIC_PAC_IR_PARAM_APPLY) == 0) {
    irCommand->apply();
    return;
  }
  //------------------ TOPIC_PAC_IR_VERSION_GET ----------------------
  if (strcmp(topic, TOPIC_PAC_IR_VERSION_GET) == 0) {
    String info = String(version) + "\n" + WiFi.localIP().toString();
    mqttClient.publish(TOPIC_PAC_IR_VERSION, info.c_str());
    return;
  }
}