#ifndef GPS_MONITOR_HPP_
#define GPS_MONITOR_HPP_

#include "SoftwareSerial.h"
#include "sfr.hpp"
#include <Arduino.h>
#include <string.h>

#include "TinyGPS++.h"

class GPSMonitor
{
public:
    GPSMonitor();

    /**
     * @brief Reads in a character from the software serial
     */
    void execute();

private:
    TinyGPSPlus gps;

    /**
     * @brief Software serial instance to communicate with the GPS receiver
     */
    SoftwareSerial ss = SoftwareSerial(constants::gps::rx_pin, constants::gps::tx_pin);

    /**
     * Begins the software serial instance and configures the GPS receiver
     */
    void init();

    // for edge case testing only
    // const char *gpsStream = "$GPGGA,045104.000,3014.1985,N,09749.2873,W,1,09,1.2,211.6,M,-22.5,M,,0000*62\r\n"
    //                         "$GPGGA,045201.000,3014.3864,N,09748.9411,W,1,10,1.2,200.8,M,-22.5,M,,0000*6C\r\n"
    //                         "$GPGG@,045252.000,3014.4273,S,09749.0628,W,1,09,1.3,206.9,M,-22.5,M,,0000*6F\r\n";
};

#endif