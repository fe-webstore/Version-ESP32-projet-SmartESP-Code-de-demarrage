#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "SmartESP32Utils.h"

const char* WIFI_SSID = "N1";
const char* WIFI_PASSWORD = "yannyann";

AsyncWebServer server(80);
String lastCommand = "";
String statusMessage = "";
String b = "true";
String ia = "";
int led = 22; // GPIO pour ESP32, remplace par le bon numéro selon ton montage

unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 1000;

void setup() {
  Serial.begin(115200);
  connectToWiFi(WIFI_SSID, WIFI_PASSWORD);
  setupWebSocket(server, &lastCommand);
  pinMode(led, OUTPUT);
}

void loop() {
  handleWebSocketCommand();
  if (millis() - lastUpdateTime > updateInterval) {
    sendAutoUpdate();
    lastUpdateTime = millis();
  }
}

void handleWebSocketCommand() {
  if (lastCommand.isEmpty()) return;

  if (lastCommand == "allume") {
    digitalWrite(led, HIGH);
    b = "true";
    ia = "OK c'est fait";
    statusMessage = "LED ALLUMÉE";
  } else if (lastCommand == "éteins") {
    digitalWrite(led, LOW);
    b = "false";
    ia = "OK je viens de l'éteindre";
    statusMessage = "LED ÉTEINTE";
  } else {
    statusMessage = "Commande inconnue";
  }

  lastCommand = "";
  sendAutoUpdate();
}

void sendAutoUpdate() {
  StaticJsonDocument<192> doc;
  doc["Ecran1"] = millis();
  doc["Ecran2"] = statusMessage;
  doc["Ecran3"] = digitalRead(led) == HIGH ? "on" : "off";
  doc["Ecran4"] = b;
  doc["Indicateur1"] = 0.70;
  doc["Indicateur2"] = 0.30;
  doc["Indicateur3"] = 0.54;
  doc["Bulb1"] = b;
  doc["Bulb2"] = "false";
  doc["Bulb3"] = "true";
  doc["Bulb4"] = "false";
  doc["notif"] = "false";
  doc["AI"] = ia;
  doc["status"] = "auto-update";
  doc["message"] = statusMessage;
  doc["Wifi"] = WiFi.SSID();
  doc["ip_address"] = WiFi.localIP().toString();
  doc["mac_address"] = WiFi.macAddress();
  doc["signal_strength"] = WiFi.RSSI();
  doc["gateway_ip"] = WiFi.gatewayIP().toString();
  doc["free_memory"] = ESP.getFreeHeap();
  doc["flash_memory"] = ESP.getFlashChipSize();

  String payload;
  serializeJson(doc, payload);
  ws.textAll(payload);
  Serial.println("🔄 Mise à jour automatique envoyée");
}
