#ifndef RADIO_CONTROLTASK_HPP_
#define RADIO_CONTROLTASK_HPP_

#include "Command.hpp"
#include "Pins.hpp"
#include "RadioLib.h"
#include "sfr.hpp"

class RadioControlTask
{
public:
    RadioControlTask();
    void execute();

private:
    RFM96 radio;
    int8_t code;
    void init();
    bool transmit(uint8_t byteArr[], size_t size);
    bool receive(uint8_t byteArr[], size_t size);
    Command *commandFactory(RawCommand raw);
};

#endif
