#include <Arduino.h>
#include "ESP32_C3_TimerInterrupt.h"

#include <EEPROM.h>
#include "bsp.hpp"
#include "hmi.hpp"
#include "screens/main_screen.hpp"
#include "screens/stats_screen.hpp"
#include "screens/splash_screen.hpp"

ESP32Timer ITimer0(0);

void setup() {
    Serial.begin(115200);
    
    bsp::init();
    auto& hmi = HMI::get();

    if (ITimer0.attachInterruptInterval(1000, [](void*)-> bool { HMI::get().timer_callback(); return true;}))
	{
		Serial.print(F("Starting  ITimer0 OK, millis() = "));
		Serial.println(millis());
	}
	else
		Serial.println(F("Can't set ITimer0. Select another freq. or timer"));

    SplashScreen splash;
    splash.render(HMI::get().hw());

    hmi.register_screen(std::move(std::make_unique<MainScreen>()));
    hmi.register_screen(std::move(std::make_unique<StatsScreen>()));
}

void loop() {
    HMI::get().update();

    delay(500);
}
