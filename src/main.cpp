#include "MainControlLoop.hpp"
#include <avr/wdt.h>

MainControlLoop mcl;

void setup()
{
    // configure watchdog timer
    wdt_disable();
    wdt_enable(WDTO_8S);
#ifdef VERBOSE
    Serial.begin(9600);
#endif

    sfr::gps::boot_time = millis();

    // sets ChipSat LED high for debugging
    pinMode(constants::led::led_pin, OUTPUT);
    digitalWrite(constants::led::led_pin, HIGH);

    // cuts off power to GPS
    pinMode(constants::gps::reset_pin, OUTPUT);
    digitalWrite(constants::gps::reset_pin, HIGH);

    // prevents current from leaking over GPS serial line (~79mA -> ~22mA)
    pinMode(constants::gps::tx_pin, OUTPUT);
    digitalWrite(constants::gps::tx_pin, LOW);

    // seed random number gen with noise from unconnected analog pin
    randomSeed(analogRead(A0));
}

void loop()
{
    mcl.execute();
}