#ifndef GPS_MONITOR_HPP_
#define GPS_MONITOR_HPP_

#include <Arduino.h>
#include <SoftwareSerial.h>

//dog size must larger than the char length of the GPGGA msg
#define DOG_SIZE 62

typedef struct GPS
{
    //extra slot for '\0' when tokenlizing
    char watch_dog[DOG_SIZE+1];

    float UTC_h;
    float UTC_m;
    float UTC_s;

    float Lat;
    float Long;
    float Alt;
} GPS;

class GPSMonitor
{
public:
    GPSMonitor();
    void execute();

private:
    GPS *myGPS;
    SoftwareSerial mySerial = SoftwareSerial(4, 3); // RX, TX
    void print_dog();
    void buffer_shift(char new_char);
    int check_GPGGA();
    float token_to_time(char* token, int precision);
    float token_to_degree(char* token, bool is_lat);
    void parse_gps();
};

#endif