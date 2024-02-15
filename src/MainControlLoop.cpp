#include "MainControlLoop.hpp"

MainControlLoop::MainControlLoop()
    : imu_monitor(),
      temp_monitor(),
      gps_monitor(),
      radio_control_task()

{
}

int freeRam()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

void MainControlLoop::execute()
{

#ifdef VERBOSE
    Serial.println(F("-------------------- START LOOP --------------------"));
    if (sfr::imu::gyro_x->get_value(&val)) {
        Serial.print(F("Gyro X: "));
        Serial.println(val);
    }
    if (sfr::imu::gyro_y->get_value(&val)) {
        Serial.print(F("Gyro Y: "));
        Serial.println(val);
    }
    if (sfr::imu::gyro_z->get_value(&val)) {
        Serial.print(F("Gyro Z: "));
        Serial.println(val);
    }
    if (sfr::imu::acc_x->get_value(&val)) {
        Serial.print(F("Accel X: "));
        Serial.println(val);
    }
    if (sfr::imu::acc_y->get_value(&val)) {
        Serial.print(F("Accel Y: "));
        Serial.println(val);
    }
    if (sfr::imu::acc_z->get_value(&val)) {
        Serial.print(F("Accel Z: "));
        Serial.println(val);
    }
    if (sfr::temperature::temp_c->get_value(&val)) {
        Serial.print(F("Temperature (C): "));
        Serial.print(val);
        Serial.println(F(" C"));
    }
    if (sfr::gps::latitude->get_value(&val)) {
        Serial.print(F("GPS Latitude: "));
        Serial.println(val);
    }
    if (sfr::gps::longitude->get_value(&val)) {
        Serial.print(F("GPS Longitude: "));
        Serial.println(val);
    }
    if (sfr::gps::altitude->get_value(&val)) {
        Serial.print(F("GPS Altitude (m): "));
        Serial.println(val);
    }
    if (sfr::gps::utc_time->get_value(&val)) {
        Serial.print("UTC Time: ");
        Serial.println(val);
    }

    Serial.print("Free RAM: ");
    Serial.println(freeRam());

#endif

    temp_monitor.execute();
    imu_monitor.execute();
    gps_monitor.execute();
    radio_control_task.execute();

#ifdef VERBOSE
    Serial.println(F("-------------------- END LOOP --------------------"));
#endif
}