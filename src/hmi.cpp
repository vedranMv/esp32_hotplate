#include "hmi.hpp"
#include <Arduino.h>
#include "ESP32_C3_TimerInterrupt.h"

const uint8_t SCREEN_WIDTH = 128; // OLED display width, in pixels
const uint8_t SCREEN_HEIGHT = 32; // OLED display height, in pixels


HMI& HMI::get()
{
    static auto encoder = ClickEncoder(10, 20, 21, 4, LOW);
    static HMI instance(Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire), encoder);
    
    return instance;
}

HMI::HMI(Adafruit_SSD1306 hw, ClickEncoder& encoder): _hw(hw), _encoder(encoder)
{
    // SSD1306 I2C display
    if(!_hw.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
    {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
    // Encoder setup
    _encoder.setAccelerationEnabled(true);
    _encoder.setDoubleClickEnabled(true);
    _encoder.setLongPressRepeatEnabled(true);
    
}


void HMI::clear()
{
    // Clear the buffer
    _hw.clearDisplay();
}

void HMI::update() 
{
    clear();
    if (_registered_screens > 0)
    {
        _screens[_active_screen]->render(_hw);
    }

    _hw.display();
}

void HMI::register_screen(std::unique_ptr<BaseApp> screen)
{
    _screens[_registered_screens] = std::move(screen);
    _registered_screens++;
}

void HMI::isr_timer_callback()
{
    _encoder.service();

    auto button = _encoder.getButton();
    auto encoder = _encoder.getIncrement();

    if (button == Button::DoubleClicked)
    {
        _active_screen = (_active_screen+1) % _registered_screens;
    }

    if (_registered_screens > 0)
    {
        _screens[_active_screen]->isr_input_callback(button, encoder);
    }
}