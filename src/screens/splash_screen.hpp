#pragma once
#include "base_screen.hpp"


class SplashScreen: public BaseScreen
{
public:

    SplashScreen() {};
    void render(Adafruit_SSD1306 &display) final
    {
        display.clearDisplay();
        display.setTextColor(SSD1306_WHITE);

        int16_t x,x1, y,y1;
        uint16_t w,w1, h,h1;
        display.setTextSize(2);
        display.getTextBounds("Hot Plate", 0, 0, &x, &y, &w, &h);
        Serial.printf("x: %d, y: %d, w: %d, h:%d\n", x, y, w, h);

        display.setTextSize(1);
        display.getTextBounds("by ESP32", 0, 0, &x1, &y1, &w1, &h1);
        Serial.printf("x1: %d, y1: %d, w1: %d, h1:%d\n", x1, y1, w1, h1);

        display.setCursor((128-w)/2,(32-h-h1)/2);

        display.setTextSize(2);
        display.printf("Hot Plate");
        display.setTextSize(1);
        display.setCursor((128-w)/2+(w-w1),(32-h-h1)/2+h);
        display.printf("by ESP32");
        display.display();
        delay(2000);
    }
    void input_callback(Button::eButtonStates btn_state, int16_t enc_increment) final {}
};