#ifndef _SENSORREADING_HPP_
#define _SENSORREADING_HPP_

#include "Arduino.h" // allows Serial.println() calls
#include "constants.hpp"

class SensorReading
{
private:
    float max;          // max valid value
    float min;          // min valid value
    bool valid = false; // if SensorReading is valid
    float value;

public:
    SensorReading(float max, float min);   // constructor
    bool get_value(float *value_location); // get SensorReading averaged value
    void set_value(float x);               // set SensorReading value
    void set_invalid();
    void set_valid();
    float get_min();
    float get_max();
    bool is_valid();
};

#endif