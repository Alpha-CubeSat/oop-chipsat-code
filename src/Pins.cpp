#include "Pins.hpp"

void Pins::setPinState(int pinName, int pinState)
{
    sfr::pins::pinMap[pinName] = pinState;
    digitalWrite(pinName, pinState);
}

int Pins::getPinState(int pinName)
{
    return sfr::pins::pinMap[pinName];
}

void Pins::setInitialPinStates()
{
    // pinMode(constants::acs::xPWMpin, OUTPUT);

    // Pins::setPinState(constants::acs::xPWMpin, LOW);
}