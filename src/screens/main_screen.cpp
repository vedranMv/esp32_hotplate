#include "screens/main_screen.hpp"

#include "bsp.hpp"

MainScreen::MainScreen() 
{
    _set_state(State::Cooling);
};

void MainScreen::render(Adafruit_SSD1306 &display)
{
    auto T = bsp::analog::get_plate_temp_C();

    run_temp_regulation();

    if (isnan(T))
    {
        return;
    }

    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);
    int16_t x, y;
    uint16_t w, h;
    display.getTextBounds("[Manual Mode]", 0, 0, &x, &y, &w, &h);
    display.setCursor(128-w,0);
    display.println("[Manual Mode]");
    display.setCursor(0,h);

    display.setTextSize(2);
    display.printf("%3d/%3d %cC\n", T, _temp_setpoint,247);
    display.setTextSize(1);

    float input_i_a = bsp::analog::get_amp_input_A();
    float input_v_v = bsp::analog::get_volt_input_V();
    
    if (_state_edit())
        display.printf("Edit setpoint\n");
    else if (_state_cooling())
        display.printf("Cooling\n");
    else if (_state_heating())
    {
        display.printf("Heating @ %01.1fW\n", input_v_v*input_i_a);
    }

    // Run fan if we're trying to cool down
    if (_state_cooling())
    {
        if (T > 40)
            bsp::output::set_fan_duty(100);
        else
            bsp::output::set_fan_duty(0);
    }
}

void MainScreen::input_callback(Button::eButtonStates btn_state, int16_t enc_increment)
{
    if (_state_edit())
    {
        _temp_setpoint = _temp_setpoint-_last_enc_increment;
        if (_temp_setpoint > 250)
        {
            _temp_setpoint = 250;
        }
        if (_temp_setpoint < 20)
        {
            _temp_setpoint = 20;
        }
    }

    switch (btn_state)
    {
    case Button::Clicked:
        Serial.println("Clicked");
        if (_state_edit())
        {
            _set_state(State::Cooling);
        }
        else 
        {
            _set_state(State::Editing);
        }
        break;
    case Button::DoubleClicked:
        Serial.println("Double Clicked");
        break;
    case Button::Held:
        break;
    case Button::LongPressRepeat:
        Serial.println("Long press");
        if (!_state_heating()){
            _set_state(State::Heating);
        }
        else if (!_state_cooling())
        {
            _set_state(State::Cooling);
        }
        
        break;
    case Button::Released:
        Serial.println("Released");
        break;
    default:
        // no output for "Open" or "Closed" to not spam the console.
        break;
    }

    _last_btn_state = btn_state; 
    _last_enc_increment = enc_increment; 
}

void MainScreen::run_temp_regulation()
{
    auto T = bsp::analog::get_plate_temp_C();
    auto temp_delta = T - _temp_setpoint;

    // Only run regulation when in heating mode
    if (!_state_heating())
    {
        return;
    }
       

    if (temp_delta >= 1 && bsp::output::get_heater_on())
    {
        bsp::output::set_heater_state(0);
    }

    if (temp_delta <= -1 && !bsp::output::get_heater_on())
    {
        bsp::output::set_heater_state(1);
    }

}

bool MainScreen::_state_edit() { return _state == State::Editing; }
bool MainScreen::_state_heating() { return _state == State::Heating; }
bool MainScreen::_state_cooling() { return _state == State::Cooling; }

void MainScreen::_set_state(State state)
{
    if (state == State::Cooling || state == State::Editing)
    {
        digitalWrite(GPIO_NUM_6, 0);
        
    }

    if (state == State::Heating)
    {
        digitalWrite(GPIO_NUM_6, 1);
    }

    _state = state;
}