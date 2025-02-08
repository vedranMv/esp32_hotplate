#include "bsp.hpp"
#include <config2eeprom.hpp>
#include "driver/temp_sensor.h"


namespace bsp::analog
{

void init()
{
    pinMode(AIN_TEMP_PLATE, ANALOG);  // Temperature
    pinMode(AIN_VOLT_IN, ANALOG);  // Input Voltage
    pinMode(AIN_AMP_IN, ANALOG);  // Input Current

    // Configure internal temperature sensor in ESP32
    temp_sensor_config_t temp_sensor = TSENS_CONFIG_DEFAULT();
    temp_sensor_get_config(&temp_sensor);
    temp_sensor.dac_offset = TSENS_DAC_DEFAULT; // DEFAULT: range:-10℃ ~  80℃, error < 1℃.
    temp_sensor_set_config(temp_sensor);
    temp_sensor_start();
}
    
float get_volt_input_V()
{
    const float scaling = 0.133419f*1000;
    return static_cast<float>(analogReadMilliVolts(AIN_VOLT_IN)) / scaling;
}

float get_amp_input_A()
{
    const float amplifier_scaling = 500.0f; // 500 mv/A
    return static_cast<float>(analogReadMilliVolts(AIN_AMP_IN)) / amplifier_scaling;
}

/**
 * @brief Get temperature from a lookup table of resistance
 * 
 * @param res 
 * @return float 
 */
float _temp_from_r(const float res)
{
  const float Tstart = 0;
  const float Tstep = 5.0f;
  const float RTempLookup[] = 
  {
    100.00f, 101.95f, 103.90f, 105.85f, 107.79f, 109.73f, 111.67f,
    113.61f, 115.54f, 117.47f, 119.40f, 121.32f, 123.24f, 125.16f,
    127.07f, 128.98f, 130.89f, 132.80f, 134.70f, 136.60f, 138.50f,
    140.39f, 142.29f, 144.17f, 146.06f, 147.94f, 149.82f, 151.70f,
    153.58f, 155.45f, 157.31f, 159.18f, 161.04f, 162.90f, 164.76f,
    166.61f, 168.47f, 170.31f, 172.16f, 174.00f, 175.84f, 177.68f
  };
  uint8_t i = 0;

  if (res <= RTempLookup[0])
    return Tstart;

  if (res >= RTempLookup[(sizeof(RTempLookup)/sizeof(float))-1])
    return Tstart+Tstep*(sizeof(RTempLookup)/sizeof(float)-1);

  for (i; RTempLookup[i] < res; i++);

  float ret_val = (Tstart+(i-1)*Tstep) + (res-RTempLookup[i-1])*(Tstep)/(RTempLookup[i]-RTempLookup[i-1]);

  return ret_val;
}

uint16_t get_plate_temp_C()
{
    // Amplifier has a gain of 5
    auto Vo = analogReadMilliVolts(AIN_TEMP_PLATE) / 5;

    auto Vref = 2918.0f;
    const auto R1 = 681.0f;
    const auto R2 = 99.9f;
    const auto R3 = 679.0f;
    auto Vfrac = Vo/Vref;
    auto R4 = (R3*(Vfrac+R2/(R2+R1)))/(1-Vfrac-R2/(R2+R1));
    auto T = _temp_from_r(R4);

    return static_cast<uint16_t>(T);
}

uint16_t get_board_temp_C()
{
    float temp_out;
    temp_sensor_read_celsius(&temp_out);

    return static_cast<uint16_t>(temp_out);
}


} // namespace bsp::analog

namespace bsp::output
{
// We use a weak FET driving circuit, which can't tolerate high frequency switching
// To prevent that, we allow FET to change state only once in a while
constexpr uint32_t MIN_FET_STATE_DELAY_MS = 300;
static uint32_t last_fet_change = 0;

static uint8_t fan_percent = 0;

void init()
{
    pinMode(OUT_FAN_PWM, OUTPUT);    // Heat FET
    pinMode(OUT_HEAT_PWM, OUTPUT);    // Fan PWM

    // Configure fan PWM
    analogWriteFrequency(25000);
    analogWrite(OUT_FAN_PWM, 0);
}

void set_heater_state(uint8_t state)
{
    auto fet_change_interval = millis() - last_fet_change;

    if (fet_change_interval < MIN_FET_STATE_DELAY_MS)
        return;

    digitalWrite(OUT_HEAT_PWM, state);
    last_fet_change = millis();
}

bool get_heater_on()
{
    return digitalRead(OUT_HEAT_PWM) == 1;
}

void set_fan_duty(uint8_t duty)
{
    float duty_to_bits = static_cast<float>(duty) * 4096.0f / 100.0f;
    analogWrite(OUT_FAN_PWM, static_cast<int>(duty_to_bits));
    fan_percent = duty;
}

uint8_t get_fan_duty()
{
    return fan_percent;
}

} // namespace bsp::output

namespace bsp::config
{

    staticConfigDoc config_map;
    // Config class and map hash
    config2eeprom c2e;

    template <typename T>
    T get(String key, T def_value)
    {
        T ret_val = def_value;
        if (config_map.containsKey(key))
        {
            ret_val = config_map[key].as<T>();
        }
        return ret_val;
    }

    template <typename T>
    void set(String key, T value)
    {
        Serial.println("Updated map");
        config_map[key] = value;
        c2e.save(config_map);
    }

    void init()
    {
        if (!c2e.load(config_map))
        {
            Serial.println("No config in EEPROM.");
        }
        else
        {
            Serial.println("Config found in EEPROM.");
        }
    }

    template void set<uint16_t>(String, uint16_t);
    template uint16_t get<uint16_t>(String, uint16_t);
} // namespace bsp::config


namespace bsp
{
    void init()
    {
        bsp::analog::init();
        bsp::output::init();
        bsp::config::init();
    }
} // namespace hw
