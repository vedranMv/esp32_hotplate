#pragma once

#include <ClickEncoder.h>
#include <Adafruit_SSD1306.h>

class BaseApp
{
public:
    virtual void render(Adafruit_SSD1306 &display) = 0;
    virtual void isr_input_callback(Button::eButtonStates btn_state, int16_t enc_increment) = 0;
};
