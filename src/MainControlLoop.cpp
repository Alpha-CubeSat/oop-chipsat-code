#include "MainControlLoop.hpp"

MainControlLoop::MainControlLoop()
    : imu_monitor(),
      temp_monitor(),
      /*gps_monitor(),*/
      radio_control_task()

{
    // delay(1000);
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
    if (sfr::temperature::temp_f->get_value(&val)) {
        Serial.print("Temperature (F): ");
        Serial.print(val);
        Serial.println(" F");
    }
    if (sfr::gps::latitude->get_value(&val)) {
        Serial.print("GPS Latitude (degrees): ");
        Serial.println(val);
    }
    if (sfr::gps::longitude->get_value(&val)) {
        Serial.print("GPS Longitude (degrees): ");
        Serial.println(val);
    }
    if (sfr::gps::altitude->get_value(&val)) {
        Serial.print("GPS Alitude (m): ");
        Serial.println(val);
    }
    if (sfr::gps::utc_h->get_value(&val)) {
        Serial.print("UTC Hour: ");
        Serial.println(val);
    }
    if (sfr::gps::utc_m->get_value(&val)) {
        Serial.print("UTC Min: ");
        Serial.println(val);
    }
    if (sfr::gps::utc_s->get_value(&val)) {
        Serial.print("UTC Sec: ");
        Serial.println(val);
    }

#endif

    temp_monitor.execute();
    imu_monitor.execute();
    // gps_monitor.execute();
    radio_control_task.execute();

#ifdef VERBOSE
    Serial.println("-------------------- END LOOP --------------------");
#endif
}