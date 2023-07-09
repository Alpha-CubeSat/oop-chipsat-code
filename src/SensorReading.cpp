#include "SensorReading.hpp"

SensorReading::SensorReading(float min, float max)
{
    this->max = max;
    this->min = min;
    value = -1;
} // constructor

bool SensorReading::get_value(float *value_location)
{
    // enough values have been accumulated to get the average
    if (value != -1 ) {
        *value_location = value;
        return 1;
    } else {
        return 0;
    }
}

void SensorReading::set_value(float x)
{
    if (x <= max && x >= min) {
        if(valid){
            value = x;
        }
    } else {
        set_invalid();
    }

} // mutator for buffer

void SensorReading::set_valid()
{
    valid = true;
}

void SensorReading::set_invalid()
{
    valid = false;
    value = -1;
}

float SensorReading::get_max()
{
    return max;
}

float SensorReading::get_min()
{
    return min;
}

bool SensorReading::is_valid()
{
    return valid;
}