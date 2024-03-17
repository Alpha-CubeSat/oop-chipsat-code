#include "TempMonitor.hpp"

TempMonitor::TempMonitor()
{
    // Init I2C communication as master
    Wire.begin();
}

void TempMonitor::execute()
{
    if (!initialized) {
        // TODO: Change
        initialized = true;
    }

    // Start I2C transmit
    Wire.beginTransmission(constants::temperature::i2c_address);
    // Select no hold master
    Wire.write(0xF3);
    Wire.endTransmission();
    delay(300);

    // Request 2 bytes of data
    Wire.requestFrom(constants::temperature::i2c_address, 2);

    if (Wire.available() == 2) {
        data[0] = Wire.read();
        data[1] = Wire.read();
    }

    // Convert the data
    uint32_t rawtemp = (data[0] * 256 + data[1]) & 0xFFFC;
    float temp_c = -46.85 + (175.72 * (rawtemp / 65536.0));

    sfr::temperature::temp_c = temp_c;
}
