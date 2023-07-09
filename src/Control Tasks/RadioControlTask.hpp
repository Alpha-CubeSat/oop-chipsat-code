#ifndef RADIO_CONTROL_TASK_HPP_
#define RADIO_CONTROL_TASK_HPP_

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
    Command *commandFactory(RawCommand raw);
    RFM96 *radio;
    void Radio_init();
};

#endif
