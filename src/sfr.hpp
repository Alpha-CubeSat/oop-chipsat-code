#ifndef SFR_HPP_
#define SFR_HPP_

#include "Arduino.h"
#include "Modes/sensor_init_mode_type.enum"
#include "Modes/sensor_mode_type.enum"
#include "SensorReading.hpp"
#include "constants.hpp"
#include <ArxContainer.h>

namespace sfr {
    namespace pins {
        extern arx::map<int, int> pinMap;
    } // namespace pins

    namespace temperature {
        extern SensorReading *temp_c;
        extern SensorReading *temp_f;
    } // namespace temperature
    
};    // namespace sfr

#endif