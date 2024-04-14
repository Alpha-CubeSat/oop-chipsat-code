#ifndef GPS_MONITOR_HPP_
#define GPS_MONITOR_HPP_

#include "sfr.hpp"
#include <Arduino.h>
#include <SoftwareSerial.h>

class GPSMonitor
{
public:
    GPSMonitor();

    /**
     * @brief Reads in a character from the software serial
     */
    void execute();

private:
    /**
     * @brief Software serial instance to communicate with the GPS receiver
     */
    SoftwareSerial ss = SoftwareSerial(constants::gps::rx_pin, constants::gps::tx_pin);

    /**
     * Begins the software serial instance and configures the GPS receiver
     */
    void init();

    /**
     * @brief Appends another character to a GPS message under construction
     *
     * @param c The character to add
     */
    bool encode(char c);

    /**
     * @brief Checks to see if the first 5 characters match those of the expected
     * full GPS message
     */
    bool check_GPGGA();

    /**
     * @brief Buffer that holds the current constructed GPS message
     */
    char term_buffer[constants::gps::buffer_size];

    /**
     * @brief The number of characters in the current term
     */
    uint8_t char_count = 0;

    /**
     * @brief The number of terms in the GPS message
     */
    uint8_t term_count = 0;

    //     const char *gpsStream =
    //   "$GPGGA,045104.000,3014.1985,N,09749.2873,W,1,09,1.2,211.6,M,-22.5,M,,0000*62\r\n"
    //   "$GPGGA,045201.000,3014.3864,N,09748.9411,W,1,10,1.2,200.8,M,-22.5,M,,0000*6C\r\n"
    //   "$GPGG@,045252.000,3014.4273,S,09749.0628,W,1,09,1.3,206.9,M,-22.5,M,,0000*6F\r\n";
};

#endif