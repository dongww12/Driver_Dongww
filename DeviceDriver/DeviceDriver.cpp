#include "DeviceDriver.h"
//#include <stdexception>

using namespace std;

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{}

class ReadFailException {

};

class WriteFailException {

};

int DeviceDriver::read(long address)
{
    constexpr int numRead = 5;
    int result = (int)(m_hardware->read(address));
    for (int i = 1; i < numRead; ++i) {
        if (result != (int)(m_hardware->read(address)))
            throw ReadFailException();
    }
    return result;
}

void DeviceDriver::write(long address, int data)
{
    if (read(address) != 0xFF)
        throw WriteFailException();
    m_hardware->write(address, (unsigned char)data);
}