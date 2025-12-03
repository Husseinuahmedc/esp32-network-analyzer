#include "oled.h"
#include <WiFi.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void initOLED() {
    u8g2.begin();
    u8g2.setBitmapMode(1);
}

void drawStatus(String status, int count) {
    u8g2.clearBuffer();

    String ip = WiFi.localIP().toString();
    if (ip == "0.0.0.0") {
        ip = "Connecting...";
    }

    u8g2.setFont(u8g2_font_6x10_tf);

    u8g2.setCursor(0, 10);
    u8g2.print("IP:");

    u8g2.setCursor(0, 22);
    u8g2.print(ip);

    u8g2.drawLine(0, 26, 128, 26);

    u8g2.setFont(u8g2_font_profont12_tr);
    u8g2.setCursor(0, 42);
    u8g2.print("State: ");
    u8g2.print(status);

    u8g2.setCursor(0, 60);
    u8g2.print("Networks:");

    u8g2.setFont(u8g2_font_logisoso16_tr);
    u8g2.setCursor(80, 62);
    u8g2.print(count);

    u8g2.sendBuffer();
}

