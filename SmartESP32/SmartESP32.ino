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



const int prsens = 4 ; 
const int light = 18 ;
const int led = 21 ;

// ------- Variables lecture capteurs --------

void setup() {
  Serial.begin(115200);
  connectToWiFi(ssid, password);
  setupWebSocket(server, &lastCommand);

  pinMode(prsens , INPUT);
    pinMode(light , OUTPUT);
     pinMode(led , OUTPUT);

  
}

void loop() {

  if (lastCommand.indexOf("tg1on") != -1  ) {
   
   digitalWrite(light, HIGH);
 

  } else  if (lastCommand.indexOf("tg1off") != -1)  {
       digitalWrite(light, LOW);



  }

  Autoupdate();
}





void Autoupdate() {
  
 processCommandIfNeeded(lastCommand, payload);
 
  if (millis() - lastUpdateTime > updateInterval) {
   

      
    sendAutoUpdate(payload);
    payload.notif = "false";
    payload.ia = "false";
    payload.ac = "";

    lastUpdateTime = millis();
  }
}
