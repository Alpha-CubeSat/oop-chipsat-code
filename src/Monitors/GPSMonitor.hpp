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
    bool check_GPGGA();

private:
    SoftwareSerial ss = SoftwareSerial(constants::gps::rx_pin, constants::gps::tx_pin);
    char term_buffer[11];
    bool valid_msg = true;
    uint8_t char_count = 0;
    uint8_t term_count = 0;
    bool encode(char c);

    //     const char *gpsStream =
    //   "$GPGGA,045104.000,3014.1985,N,09749.2873,W,1,09,1.2,211.6,M,-22.5,M,,0000*62\r\n"
    //   "$GPGGA,045201.000,3014.3864,N,09748.9411,W,1,10,1.2,200.8,M,-22.5,M,,0000*6C\r\n"
    //   "$GPGG@,045252.000,3014.4273,S,09749.0628,W,1,09,1.3,206.9,M,-22.5,M,,0000*6F\r\n";
};

#endif