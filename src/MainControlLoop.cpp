#include "MainControlLoop.hpp"

MainControlLoop::MainControlLoop()
    : imu_monitor(),
      temp_monitor(),
      radio_control_task(),
      gps_monitor()

{
    delay(1000);
}

void MainControlLoop::execute()
{

#ifdef VERBOSE
    Serial.println("-------------------- START LOOP --------------------");
    if (sfr::imu::gyro_x->get_value(&val)) {
        Serial.print("Gyro X: ");
        Serial.println(val);
    }
    if (sfr::imu::gyro_y->get_value(&val)) {
        Serial.print("Gyro Y: ");
        Serial.println(val);
    }
    if (sfr::imu::gyro_z->get_value(&val)) {
        Serial.print("Gyro Z: ");
        Serial.println(val);
    }
    if (sfr::imu::acc_x->get_value(&val)) {
        Serial.print("Accel X: ");
        Serial.println(val);
    }
    if (sfr::imu::acc_y->get_value(&val)) {
        Serial.print("Accel Y: ");
        Serial.println(val);
    }
    if (sfr::imu::acc_z->get_value(&val)) {
        Serial.print("Accel Z: ");
        Serial.println(val);
    }
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
    if (sfr::gps::latitude->get_value(&val)) {
        Serial.print("GPS Latitude (degrees): ");
        Serial.print(val);
    }
    if (sfr::gps::longitude->get_value(&val)) {
        Serial.print("GPS Longitude (degrees): ");
        Serial.print(val);
    }
    if (sfr::gps::altitude->get_value(&val)) {
        Serial.print("GPS Alitude (m): ");
        Serial.print(val);
    }
    if (sfr::gps::utc_h->get_value(&val)) {
        Serial.print("UTC Hour: ");
        Serial.print(val);
    }
    if (sfr::gps::utc_m->get_value(&val)) {
        Serial.print("UTC Min: ");
        Serial.print(val);
    }
    if (sfr::gps::utc_s->get_value(&val)) {
        Serial.print("UTC Sec: ");
        Serial.print(val);
    }
#endif

    imu_monitor.execute();
    temp_monitor.execute();
    radio_control_task.execute();
    gps_monitor.execute();

#ifdef VERBOSE
    Serial.println("-------------------- END LOOP --------------------");
#endif
}