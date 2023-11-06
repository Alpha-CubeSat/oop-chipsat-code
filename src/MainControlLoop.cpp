#include "MainControlLoop.hpp"

MainControlLoop::MainControlLoop()
    : temp_monitor()

{
    // delay(1000);
}

void MainControlLoop::execute()
{

#ifdef VERBOSE
    Serial.println("-------------------- START LOOP --------------------");
    
    if (sfr::temperature::temp_c->get_value(&val)) {
        Serial.print("Temperature (C): ");
        Serial.print(val);
        Serial.println(" C");
    }
    if (sfr::temperature::temp_c->get_value(&val)) {
        Serial.print("Temperature (F): ");
        Serial.print(val);
        Serial.println(" F");
    }
    
#endif


    temp_monitor.execute();

#ifdef VERBOSE
    Serial.println("-------------------- END LOOP --------------------");
#endif
}