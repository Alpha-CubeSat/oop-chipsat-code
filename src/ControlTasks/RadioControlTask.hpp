#ifndef RADIO_CONTROLTASK_HPP_
#define RADIO_CONTROLTASK_HPP_

// #include "Command.hpp"
#include "RadioLib.h"
#include "sfr.hpp"

class RadioControlTask
{
public:
    RadioControlTask();
    void execute();

private:
    RFM96 radio = new Module(constants::radio::radio_cs_pin, constants::radio::radio_di0_pin,
                             constants::radio::radio_rst_pin, constants::radio::radio_busy_pin);
    int8_t code;
    void init();
    bool transmit(String packet);
    bool receive(String commmand);
    // Command *commandFactory(RawCommand raw);
    String buildDownlink();
    String sensorReadingString(SensorReading *sr);
};

#endif
