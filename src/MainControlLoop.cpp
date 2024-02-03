#include "MainControlLoop.hpp"

MainControlLoop::MainControlLoop()
    : imu_monitor(),
      temp_monitor(),
      gps_monitor(),
      radio_control_task()

{
    // delay(1000);
}

int freeRam() {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
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
    if (sfr::temperature::temp_f->get_value(&val)) {
        Serial.print(F("Temperature (F): "));
        Serial.print(val);
        Serial.println(F(" F"));
    }
    if (sfr::gps::latitude_valid) {
        Serial.print("GPS Latitude (degrees): ");
        Serial.println(sfr::gps::latitude);
    }
    if (sfr::gps::longitude_valid) {
        Serial.print("GPS Longitude (degrees): ");
        Serial.println(sfr::gps::longitude);
    }
    if (sfr::gps::altitude->get_value(&val)) {
        Serial.print(F("GPS Alitude (m): "));
        Serial.println(val);
    }
    if (sfr::gps::utc_time_valid) {
        Serial.print("UTC Time: ");
        Serial.println(sfr::gps::utc_time);
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