#include "MainControlLoop.hpp"
#include <avr/wdt.h>

MainControlLoop mcl;

void setup()
{
    wdt_disable();
    wdt_enable(WDTO_8S);
#ifdef VERBOSE
    Serial.begin(9600);
#endif
    pinMode(constants::gps::reset_pin, OUTPUT);
    digitalWrite(constants::gps::reset_pin, HIGH);
    sfr::gps::boot_time = millis();

    // seed random number gen with noise from unconnected analog pin
    randomSeed(analogRead(A0));
}

void loop()
{
    mcl.execute();
}