#pragma once
#include "base_app.hpp"


class ManualMode: public BaseApp
{
public:
    enum class State
    {
        Cooling = 0,
        Heating,
        Editing
    };

    ManualMode();
    void render(Adafruit_SSD1306 &display) final;
    void input_callback(Button::eButtonStates btn_state, int16_t enc_increment) final;

private:
    bool _state_edit();
    bool _state_heating();
    bool _state_cooling();
    void _set_state(State state);

    void run_temp_regulation();


    uint32_t    _last_update_tick_ms = 0;
    State       _state = State::Cooling;

    uint16_t _temp_setpoint = 250;

    Button::eButtonStates _last_btn_state;
    int16_t _last_enc_increment;
};