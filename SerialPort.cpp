#include "SerialPort.h"
#include <iostream>

SerialPort::SerialPort(const std::string& portName_, int baudRate_)
    : portName(portName_), baudRate(baudRate_), openFlag(false) {}

bool SerialPort::open() {
    std::cout << "Stub: Opening serial port " << portName << " at " << baudRate << " baud." << std::endl;
    openFlag = true;
    return true;
}

void SerialPort::close() {
    std::cout << "Stub: Closing serial port " << portName << std::endl;
    openFlag = false;
}

bool SerialPort::isOpen() const {
    return openFlag;
}

void SerialPort::write(const std::string& data) {
    std::cout << "Stub: Writing to serial port: " << data;
}

std::string SerialPort::readLine() {
    std::cout << "Stub: Reading line from serial port..." << std::endl;
    return "ok\n";
}
