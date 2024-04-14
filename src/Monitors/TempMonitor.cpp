#include "TempMonitor.hpp"

TempMonitor::TempMonitor()
{
    // Init I2C communication as master
    Wire.begin();
}

void TempMonitor::execute()
{
    // Start I2C transmit
    Wire.beginTransmission(constants::temperature::i2c_address);
    // Select no hold master
    Wire.write(0xF3);
    Wire.endTransmission();
    delay(300); // TODO: Look at this delay

    // Request 2 bytes of data
    Wire.requestFrom(constants::temperature::i2c_address, 2);
    // TODO: Delay here?

    if (Wire.available() == 2) {
        data[0] = Wire.read();
        data[1] = Wire.read();
    }

    // Convert the data
    uint32_t rawtemp = (data[0] * 256 + data[1]) & 0xFFFC;
    sfr::temperature::temp_c = -46.85 + (175.72 * (rawtemp / 65536.0));
}
