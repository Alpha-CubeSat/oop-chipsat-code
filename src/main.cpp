#include "MainControlLoop.hpp"
#include <avr/wdt.h>

MainControlLoop mcl;

void setup()
{
    // Set up watchdog timer
    wdt_disable();
    wdt_enable(WDTO_8S);
#ifdef VERBOSE
    Serial.begin(9600);
#endif

    // Set I2C transaction timeout
    Wire.begin();
    Wire.setWireTimeout(3000);

    sfr::gps::boot_time = millis();

    // Set ChipSat LED high for debugging
    pinMode(constants::led::led_pin, OUTPUT);
    digitalWrite(constants::led::led_pin, HIGH);

    // Cut off power to GPS
    pinMode(constants::gps::reset_pin, OUTPUT);
    digitalWrite(constants::gps::reset_pin, HIGH);

    // Prevents current from leaking over GPS serial line (~79mA -> ~22mA)
    pinMode(constants::gps::tx_pin, OUTPUT);
    digitalWrite(constants::gps::tx_pin, LOW);

    // Seed random number gen with noise from unconnected analog pin
    randomSeed(analogRead(A0));
}

void loop()
{
    mcl.execute();
}