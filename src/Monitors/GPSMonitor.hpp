#ifndef GPS_MONITOR_HPP_
#define GPS_MONITOR_HPP_

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "sfr.hpp"
#include "SensorReading.hpp"


class GPSMonitor
{
public:
    GPSMonitor();
    void execute();

private:
    SoftwareSerial mySerial = SoftwareSerial(4, 3); // RX, TX
    void print_dog();
    void buffer_shift(char new_char);
    int check_GPGGA();
    float token_to_time(char *token, int precision);
    float token_to_degree(char *token, bool is_lat);
    void parse_gps();
};

#endif