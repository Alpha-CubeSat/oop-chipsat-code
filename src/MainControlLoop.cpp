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
    Serial.println("-------------------- START LOOP --------------------");

    Serial.print("Gyro X: ");
    Serial.println(sfr::imu::gyro_x);

    Serial.print("Gyro Y: ");
    Serial.println(sfr::imu::gyro_y);

    Serial.print("Gyro Z: ");
    Serial.println(sfr::imu::gyro_z);

    Serial.print("Accel X: ");
    Serial.println(sfr::imu::acc_x);

    Serial.print("Accel Y: ");
    Serial.println(sfr::imu::acc_y);

    Serial.print("Accel Z: ");
    Serial.println(sfr::imu::acc_z);

    Serial.print("Mag X: ");
    Serial.println(sfr::imu::mag_x);

    Serial.print("Mag Y: ");
    Serial.println(sfr::imu::mag_y);

    Serial.print("Mag Z: ");
    Serial.println(sfr::imu::mag_z);

    Serial.print("Temperature (C): ");
    Serial.print(sfr::temperature::temp_c);
    Serial.println(" C");

    Serial.print("GPS On: ");
    Serial.println(sfr::gps::on);

    Serial.print("UTC Time: ");
    Serial.println(sfr::gps::utc_time);

    Serial.print("GPS Latitude: ");
    Serial.println(sfr::gps::latitude);

    Serial.print("GPS Longitude: ");
    Serial.println(sfr::gps::longitude);

    Serial.print("GPS Altitude (m): ");
    Serial.println(sfr::gps::altitude);

    Serial.print("Downlink Slot: ");
    Serial.println(sfr::radio::downlink_slot);

    Serial.print("Alive Time: ");
    Serial.println(millis() - sfr::gps::boot_time);

#endif

    temp_monitor.execute();
    imu_monitor.execute();
    gps_monitor.execute();
    radio_control_task.execute();

    // Turn off LED 5 seconds after boot
    if (millis() - sfr::gps::boot_time > constants::led::led_on_time) {
        digitalWrite(constants::led::led_pin, LOW);
    }

    wdt_reset();

#ifdef VERBOSE
    Serial.println(F("-------------------- END LOOP --------------------"));
#endif
}