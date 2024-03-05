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
    bool transmit(uint8_t *packet, uint8_t size);
    bool receive();
    bool executeDownlink();
    uint8_t map_range(float value, int min, int max);
    uint16_t map_range_16(float value, int min, int max);
    void processUplink();
    uint8_t *received;
};

#endif
