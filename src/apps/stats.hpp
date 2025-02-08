#pragma once
#include "base_app.hpp"

#include "bsp.hpp"

class Stats: public BaseApp
{
public:

    Stats() {};
    void render(Adafruit_SSD1306 &display) final
    {
        display.setTextColor(SSD1306_WHITE);

        display.setTextSize(1);
        int16_t x, y;
        uint16_t w, h;
        display.getTextBounds("[Stats]", 0, 0, &x, &y, &w, &h);
        display.setCursor(128-w,0);
        display.println("[Stats]");
        display.setCursor(0,h);

        float input_i_a = bsp::analog::get_amp_input_A();
        float input_v_v = bsp::analog::get_volt_input_V();
        display.printf("Power: %02.1fV, %01.1f\n", input_v_v, input_i_a);

        auto T = bsp::analog::get_plate_temp_C();

        display.printf("Heater: OFF, %3d %cC\n", T, 247);
        display.printf("Fan: %03d %%, %2d%cC\n", bsp::output::get_fan_duty(), bsp::analog::get_board_temp_C(),247);
    }

    void isr_input_callback(Button::eButtonStates btn_state, int16_t enc_increment) final
    {

    }
};