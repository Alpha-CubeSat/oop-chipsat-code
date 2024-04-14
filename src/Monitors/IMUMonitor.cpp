#include "IMUMonitor.hpp"

IMUMonitor::IMUMonitor()
{
}

void IMUMonitor::execute()
{
    if (!sfr::imu::initialized) {
#ifdef VERBOSE
        Serial.println(F("Turning on IMU"));
#endif
        if (!IMU.begin()) {
#ifdef VERBOSE
            Serial.println(F("IMU failed"));
#endif
            sfr::imu::initialized = false;
        } else {
            sfr::imu::initialized = true;
        }

    } else {
        capture_imu_values();
    }
}

void IMUMonitor::capture_imu_values()
{
    // TODO: Add magnetometer readings
    if (IMU.gyroscopeAvailable()) { // check if the gyroscope has new data available
        IMU.readGyroscope(
            sfr::imu::gyro_x,
            sfr::imu::gyro_y,
            sfr::imu::gyro_z); // data is in degrees/s
    }

    if (IMU.accelerationAvailable()) { // check if accelerometer is available
        IMU.readAcceleration(
            sfr::imu::acc_x,
            sfr::imu::acc_y,
            sfr::imu::acc_z); // data is in m/s^2
    }
}