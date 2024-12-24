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
    // Check if the gyroscope, accelerometer, or magnetometer has new data available
    if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(
            sfr::imu::gyro_x,
            sfr::imu::gyro_y,
            sfr::imu::gyro_z); // Data is in degrees/s
    }

    if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(
            sfr::imu::acc_x,
            sfr::imu::acc_y,
            sfr::imu::acc_z); // Data is in m/s^2
    }

    if (IMU.magneticFieldAvailable()) {
        IMU.readMagneticField(
            sfr::imu::mag_x,
            sfr::imu::mag_y,
            sfr::imu::mag_z); // Data in uT
    }
}