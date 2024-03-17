#include "MainControlLoop.hpp"
#include <avr/wdt.h>

MainControlLoop mcl;

void setup()
{
    wdt_disable();
    wdt_enable(WDTO_8S);

    Serial.begin(9600);
}

void loop()
{
    mcl.execute();
}