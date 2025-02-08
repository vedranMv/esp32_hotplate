#pragma once 

#include <array>
#include <memory>
#include <stdint.h>

#include "apps/base_app.hpp"


class HMI
{
    public:
        static HMI& get();

        void clear();
        void update();
        Adafruit_SSD1306& hw() { return _hw; }

        void register_screen(std::unique_ptr<BaseApp> screen);
        void isr_timer_callback();

    private:
        HMI(Adafruit_SSD1306 hw, ClickEncoder& encoder);

    Adafruit_SSD1306 _hw;
    ClickEncoder& _encoder;
    std::array<std::unique_ptr<BaseApp>, 5>_screens;
    uint8_t _registered_screens = 0;
    uint8_t _active_screen = 0;
};

class Screen
{


};