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
    Command *commandFactory(RawCommand raw);
    RFM96 *radio;
    void Radio_init();
    int8_t code;
    void transmit(byte byteArr[]);
    void receive();
};

#endif
