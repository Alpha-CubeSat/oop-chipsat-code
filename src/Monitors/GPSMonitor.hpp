#ifndef GPS_MONITOR_HPP_
#define GPS_MONITOR_HPP_

#include "SensorReading.hpp"
#include "sfr.hpp"
#include <Arduino.h>
#include <SoftwareSerial.h>

class GPSMonitor
{
public:
    GPSMonitor();
    void execute();

private:
    SoftwareSerial ss = SoftwareSerial(4, 3); // RX, TX
    char term_buffer[15];
    uint8_t char_count = 0;
    uint8_t term_count = 0;
    bool encode(char c);

};

#endif