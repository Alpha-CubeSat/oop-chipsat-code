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
#ifdef VERBOSE
        Serial.println(F("Turning on IMU"));
#endif

        IMUMonitor::IMU_init();
        if (sfr::imu::init_mode == sensor_init_mode_type::complete) {
            transition_to_normal();
            sfr::imu::initialized = true;
#ifdef VERBOSE
            Serial.println(F("IMU on"));
#endif
        } else {
            if (sfr::imu::failed_times == sfr::imu::failed_limit) {
                sfr::imu::failed_times = 0; // reset
                transition_to_abnormal_init();
#ifdef VERBOSE
                Serial.println(F("IMU failed"));
#endif
            } else {
                sfr::imu::failed_times = sfr::imu::failed_times + 1;
                // Serial.print(F("IMU initialization failed times: "));
                // Serial.println(sfr::imu::failed_times);
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

    // if (IMU.gyroscopeAvailable()) { // check if the gyroscope has new data available
    //     IMU.readGyroscope(
    //         sfr::imu::gyro_x,
    //         sfr::imu::gyro_y,
    //         sfr::imu::gyro_z); // data is in degrees/s
    // }

    // if (IMU.accelerationAvailable()) { // check if accelerometer is available
    //     IMU.readAcceleration(
    //         sfr::imu::acc_x,
    //         sfr::imu::acc_y,
    //         sfr::imu::acc_z); // data is in m/s^2
    // }
}

void IMUMonitor::transition_to_normal()
{
    sfr::imu::mode = sensor_mode_type::normal;
}

void IMUMonitor::transition_to_abnormal_init()
{
    sfr::imu::mode = sensor_mode_type::abnormal_init;

    sfr::imu::initialized = false;
    sfr::imu::init_mode = sensor_init_mode_type::init;
}
