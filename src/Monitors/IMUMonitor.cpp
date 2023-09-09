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
    if (sfr::imu::turn_on == true && sfr::imu::powered == false) {
        Serial.println("Turned on IMU");

        IMUMonitor::IMU_init();
        if (sfr::imu::init_mode == sensor_init_mode_type::complete) {
            sfr::imu::turn_on = false;
            transition_to_normal();
            sfr::imu::powered = true;
        } else {
            if (sfr::imu::failed_times == sfr::imu::failed_limit) {
                sfr::imu::failed_times = 0; // reset
                transition_to_abnormal_init();
            } else {
                sfr::imu::failed_times = sfr::imu::failed_times + 1;
                Serial.print("IMU initialization failed times: ");
                Serial.println(sfr::imu::failed_times);
                sfr::imu::init_mode = sensor_init_mode_type::init;
            }
        }
    }

    if (sfr::imu::powered == true) {
        Serial.println("IMU is on and initialized");
        capture_imu_values();
    }
}

void IMUMonitor::capture_imu_values()
{
    // fix this eeeek
    float gyroX, gyroY, gyroZ;

    float accX, accY, accZ;

    if (IMU.gyroscopeAvailable()) {             // check if the gyroscope has new data available
        IMU.readGyroscope(gyroX, gyroY, gyroZ); // library function to read from the gyroscope

        // Print Gyroscope Values
        Serial.println("Reading Gyroscopic Values");
        Serial.print("X: ");
        Serial.println(gyroX);
        Serial.print("Y: ");
        Serial.println(gyroY);
        Serial.print("Z ");
        Serial.println(gyroZ);

        sfr::imu::gyro_x->set_value(gyroX);
        sfr::imu::gyro_y->set_value(gyroY);
        sfr::imu::gyro_z->set_value(gyroZ);
    }

    if (IMU.accelerationAvailable()) { // check if accelerometer is available
        IMU.readAcceleration(accX, accY, accZ);

        // Print Accelerometer Values
        Serial.println("Reading Accelerometer Values");
        Serial.print("X: ");
        Serial.println(accX);
        Serial.print("Y ");
        Serial.println(accY);
        Serial.print("Z ");
        Serial.println(accZ);

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
    sfr::imu::acc_y->set_valid();
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

    sfr::imu::turn_on = false;
    sfr::imu::powered = false;
    sfr::imu::init_mode = sensor_init_mode_type::init;
}
