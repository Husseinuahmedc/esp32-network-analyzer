#include "FileS.h"
#include <LittleFS.h>
#include <Arduino.h>

void initLittleFS() {
    if (!LittleFS.begin()) {
        Serial.println("LittleFS mount failed");
    } else {
        Serial.println("LittleFS mounted");
    }
}

