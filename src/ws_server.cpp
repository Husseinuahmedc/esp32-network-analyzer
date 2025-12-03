#include "ws_server.h"
#include <LittleFS.h>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void initServer() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/index.html", "text/html");
    });

    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/style.css", "text/css");
    });

    server.on("/app.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/app.js", "text/javascript");
    });

    server.on("/doc.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/doc.html", "text/html");
    });

    server.addHandler(&ws);

    server.begin();
    Serial.println("HTTP server started");
}

void cleanupClients() {
    ws.cleanupClients();
}

void sendScanData(String json) {
    ws.textAll(json);
}

