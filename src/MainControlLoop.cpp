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

    Serial.print(F("Gyro X: "));
    Serial.println(sfr::imu::gyro_x);

    Serial.print(F("Gyro Y: "));
    Serial.println(sfr::imu::gyro_y);

    Serial.print(F("Gyro Z: "));
    Serial.println(sfr::imu::gyro_z);

    Serial.print(F("Accel X: "));
    Serial.println(sfr::imu::acc_x);

    Serial.print(F("Accel Y: "));
    Serial.println(sfr::imu::acc_y);

    Serial.print(F("Accel Z: "));
    Serial.println(sfr::imu::acc_z);

    Serial.print(F("Temperature (C): "));
    Serial.print(sfr::temperature::temp_c);
    Serial.println(F(" C"));

    Serial.print(F("GPS Latitude: "));
    Serial.println(sfr::gps::latitude);

    Serial.print(F("GPS Longitude: "));
    Serial.println(sfr::gps::longitude);

    Serial.print(F("GPS Altitude (m): "));
    Serial.println(sfr::gps::altitude);

    Serial.print("UTC Time: ");
    Serial.println(sfr::gps::utc_time);

    Serial.print("Free RAM: ");
    Serial.println(freeRam());

#endif

    temp_monitor.execute();
    imu_monitor.execute();
    gps_monitor.execute();
    radio_control_task.execute();

    wdt_reset();

    // delay(5000);
    // Serial.println("you should not be seeing this");

#ifdef VERBOSE
    Serial.println(F("-------------------- END LOOP --------------------"));
#endif
}