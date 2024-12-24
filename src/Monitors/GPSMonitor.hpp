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
     * @brief Begins the software serial instance and configures the GPS receiver
     */
    void init();
};

#endif