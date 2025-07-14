#include "SmartESP32Utils.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "N1";
const char* password = "yannyann";

AsyncWebServer server(80);
String lastCommand = "";
AutoUpdatePayload payload;
unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 500;

void setup() {
  Serial.begin(115200);
  connectToWiFi(ssid, password);
  setupWebSocket(server, &lastCommand);
  pinMode(23, OUTPUT);  // Ex: GPIO23 (ESP32) au lieu de D1
}

void loop() {
  Smartcommande();
  Autoupdate();
}

void Smartcommande() {
  if (!lastCommand.isEmpty()) {
    if (lastCommand == "on") {
      digitalWrite(23, HIGH);
      payload.bulb1 = "true";
      payload.notif = "LED ON 💡";
    } else if (lastCommand == "off") {
      digitalWrite(23, LOW);
      payload.bulb1 = "false";
     
      payload.notif = "LED OFF ❌";
    } else {
     
      payload.notif = "false";
    }

    lastCommand = "";
    sendAutoUpdate(payload);
    payload.notif = "false";
    payload.ia = "false";
  }
}

void Autoupdate() {
  if (millis() - lastUpdateTime > updateInterval) {
    payload.ecran1 = String(millis());
    sendAutoUpdate(payload);
    lastUpdateTime = millis();
  }
}
