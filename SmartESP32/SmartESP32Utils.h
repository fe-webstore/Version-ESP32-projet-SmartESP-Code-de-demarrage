#ifndef SMART_ESP_UTILS_H
#define SMART_ESP_UTILS_H

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

/* ----------- WebSocket Global ----------- */
AsyncWebSocket ws("/ws");

/* ----------- Connexion Wi-Fi ----------- */
void connectToWiFi(const char* ssid, const char* password) {
  WiFi.begin(ssid, password);
  Serial.print("Connexion au WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

/* ----------- Événement WebSocket ----------- */
void onWebSocketEvent(
  AsyncWebSocket* server,
  AsyncWebSocketClient* client,
  AwsEventType type,
  void* arg,
  uint8_t* data,
  size_t len,
  String* lastCommandPtr
) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("Client WebSocket #%u connecté depuis %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("Client WebSocket #%u déconnecté\n", client->id());
      break;
    case WS_EVT_DATA:
      AwsFrameInfo* info = (AwsFrameInfo*)arg;
      if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;
        *lastCommandPtr = (char*)data;
        Serial.printf("Commande reçue : %s\n", (char*)data);
      }
      break;
  }
}

/* ----------- Initialisation WebSocket ----------- */
void setupWebSocket(AsyncWebServer& server, String* lastCommandPtr) {
  ws.onEvent([lastCommandPtr](AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
    onWebSocketEvent(server, client, type, arg, data, len, lastCommandPtr);
  });
  server.addHandler(&ws);
  server.begin();
}

#endif
