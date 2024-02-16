#ifndef RADIO_CONTROLTASK_HPP_
#define RADIO_CONTROLTASK_HPP_

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
    int16_t code;
    void init();
    bool transmit(uint8_t *packet);
    bool receive();
    // Command *commandFactory(RawCommand raw);
    bool executeDownlink();
    static uint8_t serialize(SensorReading *valueObj);
    String sensorReadingString(SensorReading *sr);
};

#endif
