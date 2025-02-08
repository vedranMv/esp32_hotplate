#pragma once
#include <Arduino.h>

namespace bsp {

namespace analog
{
    const gpio_num_t AIN_TEMP_PLATE = GPIO_NUM_4;
    const gpio_num_t AIN_VOLT_IN = GPIO_NUM_2;
    const gpio_num_t AIN_AMP_IN = GPIO_NUM_3;


    void init();
    float get_volt_input_V();
    float get_amp_input_A();
    uint16_t get_plate_temp_C();
    uint16_t get_board_temp_C();

} // namespace analog

namespace output
{
    const gpio_num_t OUT_FAN_PWM = GPIO_NUM_7;
    const gpio_num_t OUT_HEAT_PWM = GPIO_NUM_6;

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