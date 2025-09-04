#pragma once
#include "SerialPort.h"
#include <string>
#include <vector>

class GrblStreamer {
public:
    GrblStreamer(SerialPort& port);
    bool streamFile(const std::string& filename);
private:
    SerialPort& serial;
    bool sendLine(const std::string& line);
};
