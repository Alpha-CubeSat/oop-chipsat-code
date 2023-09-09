#ifndef RADIO_MONITOR_HPP_
#define RADIO_MONITOR_HPP_

#include "Command.hpp"
#include "Pins.hpp"
#include "RadioLib.h"
#include "sfr.hpp"

class RadioMonitor
{
public:
    RadioMonitor();
    void execute();

private:
    RFM96 *radio;
    int8_t code;
    void init();
    bool transmit(byte byteArr[], int size);
    bool receive(byte byteArr[], int size);
    Command *commandFactory(RawCommand raw);
};

#endif
