#include <Arduino.h>
#include "scan.h"
#include "oled.h"
#include "FileS.h"
#include "ws_server.h"

#define BLUE_LED_PIN 23
#define RED_LED_PIN  25

unsigned long lastScanTime      = 0;
const unsigned long scanInterval = 10000;

void setup() {
    Serial.begin(115200);

    pinMode(BLUE_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);

    digitalWrite(BLUE_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);

    initLittleFS();
    initOLED();
    initWiFi();
    initServer();

    drawStatus("Ready", 0);
}

void loop() {
    cleanupClients();

    if (millis() - lastScanTime > scanInterval) {
        digitalWrite(BLUE_LED_PIN, HIGH);
        digitalWrite(RED_LED_PIN, LOW);

        drawStatus("Scanning...", 0);

        String data = getScanResults();

        int count = 0;
        for (unsigned int i = 0; i < data.length(); ++i) {
            if (data[i] == '{') count++;
        }

        drawStatus("Done", count);
        sendScanData(data);
        lastScanTime = millis();

        digitalWrite(BLUE_LED_PIN, LOW);
        digitalWrite(RED_LED_PIN, HIGH);
    }
}

