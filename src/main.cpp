#include "MainControlLoop.hpp"
#include <avr/wdt.h>

MainControlLoop mcl;

#ifndef UNIT_TEST

void setup()
{
    wdt_disable();
    wdt_enable(WDTO_8S);
    Serial.begin(9600);
    Serial.println("in setup");
}

void loop()
{
    mcl.execute();
}
#endif