// serial.h
#ifndef SERIAL_H
#define SERIAL_H

#include <string>

class Serial {
public:
    Serial(const std::string& port, int baudrate);
    ~Serial();
    bool write(const std::string& data);
    std::string read();

private:
    std::string port_;
    int baudrate_;
#ifdef _WIN32
    void* hSerial_; // HANDLE on Windows
#else
    int fd_;
#endif
};

#endif
