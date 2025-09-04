#include "GrblStreamer.h"
#include "SerialPort.h"
#include <iostream>

int main() {
    SerialPort serial("/dev/ttyUSB0", 115200);
    if (!serial.open()) {
        std::cerr << "Failed to open serial port." << std::endl;
        return 1;
    }

    GrblStreamer grbl(serial);
    if (!grbl.streamFile("example.gcode")) {
        std::cerr << "Streaming failed." << std::endl;
        return 1;
    }

    serial.close();
    std::cout << "Streaming complete." << std::endl;
    return 0;
}
