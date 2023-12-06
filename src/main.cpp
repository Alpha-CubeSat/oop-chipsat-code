#include "MainControlLoop.hpp"

MainControlLoop mcl;

#ifndef UNIT_TEST

void setup()
{
    // delay for 2 hours
    // delay(7200000);
    Serial.begin(9600);
    delay(5000);
}

void loop()
{
    mcl.execute();
}
#endif