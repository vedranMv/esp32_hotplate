#pragma once
#include <cstdint>
#include <WString.h>

namespace bsp {

namespace analog
{
    void init();
    float get_volt_input_V();
    float get_amp_input_A();
    uint16_t get_plate_temp_C();
    uint16_t get_board_temp_C();

} // namespace analog

namespace output
{
    void init();
    void set_heater_state(uint8_t state);
    bool get_heater_on();

    void set_fan_duty(uint8_t duty);
    uint8_t get_fan_duty();

} // namespace output

namespace config
{

    template <typename T>
    T get(String key, T def_value);

    /**
     * @brief Set config parameter and save it to non-volatile storage
     *      Must not be called from ISR context!
     * @tparam T 
     * @param key Config parameter key
     * @param value Config parameter value
     */
    template <typename T>
    void set(String key, T value);

}

    void init();

}   // namespace bsp