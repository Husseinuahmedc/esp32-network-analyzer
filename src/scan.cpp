#include "scan.h"

const char* ssid     = ""; //replace it with your ssid 
const char* password = ""; //replace it with your password

static const char* authModeToString(wifi_auth_mode_t mode) {
    switch (mode) {
        case WIFI_AUTH_OPEN:            return "Open";
        case WIFI_AUTH_WEP:             return "WEP";
        case WIFI_AUTH_WPA_PSK:         return "WPA";
        case WIFI_AUTH_WPA2_PSK:        return "WPA2";
        case WIFI_AUTH_WPA_WPA2_PSK:    return "WPA/WPA2";
        case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2-Enterprise";
        case WIFI_AUTH_WPA3_PSK:        return "WPA3";
        case WIFI_AUTH_WPA2_WPA3_PSK:   return "WPA2/WPA3";
        case WIFI_AUTH_WAPI_PSK:        return "WAPI";
        default:                        return "Unknown";
    }
}

void initWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("Connected. IP: ");
    Serial.println(WiFi.localIP());
}

String getScanResults() {
    int n = WiFi.scanNetworks();
    Serial.printf("Found %d networks\n", n);

    DynamicJsonDocument doc(8192);
    JsonArray array = doc.to<JsonArray>();

    for (int i = 0; i < n; ++i) {
        JsonObject obj = array.createNestedObject();

        String ssidStr = WiFi.SSID(i);
        if (ssidStr.length() == 0) {
            ssidStr = "(hidden)";
        }

        wifi_auth_mode_t mode = WiFi.encryptionType(i);
        int ch = WiFi.channel(i);

        obj["ssid"]    = ssidStr;
        obj["rssi"]    = WiFi.RSSI(i);
        obj["auth"]    = authModeToString(mode);
        obj["channel"] = ch;
        obj["ch"]      = ch;
    }

    String output;
    serializeJson(array, output);
    WiFi.scanDelete();

    return output;
}

