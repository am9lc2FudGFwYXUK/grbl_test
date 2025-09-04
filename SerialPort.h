#pragma once
#include <string>

class SerialPort {
public:
    SerialPort(const std::string& portName, int baudRate);
    bool open();
    void close();
    bool isOpen() const;
    void write(const std::string& data);
    std::string readLine();
private:
    std::string portName;
    int baudRate;
    bool openFlag;
};
