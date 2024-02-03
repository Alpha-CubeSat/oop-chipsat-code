#include "IMUMonitor.hpp"

IMUMonitor::IMUMonitor()
{
}

void IMUMonitor::IMU_init()
{
    if (sfr::imu::init_mode == sensor_init_mode_type::init) {
        if (!IMU.begin()) {
            sfr::imu::init_mode = sensor_init_mode_type::failed;
        } else {
            sfr::imu::init_mode = sensor_init_mode_type::complete;
        }
    }
}

void IMUMonitor::execute()
{
    if (!sfr::imu::initialized) {
        Serial.println(F("Turning on IMU"));

        IMUMonitor::IMU_init();
        if (sfr::imu::init_mode == sensor_init_mode_type::complete) {
            transition_to_normal();
            sfr::imu::initialized = true;
            Serial.println(F("IMU on"));
        } else {
            if (sfr::imu::failed_times == sfr::imu::failed_limit) {
                sfr::imu::failed_times = 0; // reset
                transition_to_abnormal_init();
                Serial.println(F("IMU failed"));
            } else {
                sfr::imu::failed_times = sfr::imu::failed_times + 1;
                Serial.print(F("IMU initialization failed times: "));
                Serial.println(sfr::imu::failed_times);
                sfr::imu::init_mode = sensor_init_mode_type::init;
            }
        }
    }

    if (sfr::imu::initialized) {
        capture_imu_values();
    }
}

void IMUMonitor::capture_imu_values()
{

    if (IMU.gyroscopeAvailable()) {             // check if the gyroscope has new data available
        IMU.readGyroscope(gyroX, gyroY, gyroZ); // library function to read from the gyroscope
        // Serial.print("Gyro X: ");
        // Serial.println(gyroX);
        sfr::imu::gyro_x->set_value(gyroX);
        sfr::imu::gyro_y->set_value(gyroY);
        sfr::imu::gyro_z->set_value(gyroZ);
    }

    if (IMU.accelerationAvailable()) { // check if accelerometer is available
        IMU.readAcceleration(accX, accY, accZ);

        sfr::imu::acc_x->set_value(accX);
        sfr::imu::acc_y->set_value(accY);
        sfr::imu::acc_z->set_value(accZ);
    }
}

void IMUMonitor::transition_to_normal()
{
    sfr::imu::mode = sensor_mode_type::normal;
    sfr::imu::gyro_x->set_valid();
    sfr::imu::gyro_y->set_valid();
    sfr::imu::gyro_z->set_valid();
    sfr::imu::acc_x->set_valid();
    sfr::imu::acc_y->set_valid();
    sfr::imu::acc_z->set_valid();
}

void IMUMonitor::transition_to_abnormal_init()
{
    sfr::imu::mode = sensor_mode_type::abnormal_init;
    sfr::imu::gyro_x->set_invalid();
    sfr::imu::gyro_y->set_invalid();
    sfr::imu::gyro_z->set_invalid();
    sfr::imu::acc_x->set_invalid();
    sfr::imu::acc_y->set_invalid();
    sfr::imu::acc_y->set_invalid();

    sfr::imu::initialized = false;
    sfr::imu::init_mode = sensor_init_mode_type::init;
}
