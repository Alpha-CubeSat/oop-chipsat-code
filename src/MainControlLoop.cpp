#include "MainControlLoop.hpp"

MainControlLoop::MainControlLoop()
    : imu_monitor(),
      temp_monitor(),
      gps_monitor(),
      radio_control_task()

{
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

    Serial.print(F("Mag X: "));
    Serial.println(sfr::imu::mag_x);

    Serial.print(F("Mag Y: "));
    Serial.println(sfr::imu::mag_y);

    Serial.print(F("Mag Z: "));
    Serial.println(sfr::imu::mag_z);

    Serial.print(F("Temperature (C): "));
    Serial.print(sfr::temperature::temp_c);
    Serial.println(F(" C"));

    Serial.print(F("GPS On: "));
    Serial.println(sfr::gps::on);

    Serial.print("UTC Time: ");
    Serial.println(sfr::gps::utc_time);

    Serial.print(F("GPS Latitude: "));
    Serial.println(sfr::gps::latitude);

    Serial.print(F("GPS Longitude: "));
    Serial.println(sfr::gps::longitude);

    Serial.print(F("GPS Altitude (m): "));
    Serial.println(sfr::gps::altitude);

    Serial.print("Downlink Slot: ");
    Serial.println(sfr::radio::downlink_slot);

#endif

    temp_monitor.execute();
    imu_monitor.execute();
    gps_monitor.execute();
    radio_control_task.execute();

    if (millis() - sfr::gps::boot_time > constants::led::led_on_time) {
        digitalWrite(constants::led::led_pin, LOW);
    }

    wdt_reset();

#ifdef VERBOSE
    Serial.println(F("-------------------- END LOOP --------------------"));
#endif
}