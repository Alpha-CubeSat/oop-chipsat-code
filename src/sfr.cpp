#include "sfr.hpp"

namespace sfr {
    namespace pins {
        arx::map<int, int> pinMap = {};

    } // namespace pins
    
    namespace temperature {
        SensorReading *temp_c = new SensorReading(-5000, 5000);
        SensorReading *temp_f = new SensorReading(-10000, 10000);

    } // namespace temperature
    
}; // namespace sfr
