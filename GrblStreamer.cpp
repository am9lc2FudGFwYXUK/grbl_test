#include "GrblStreamer.h"
#include <fstream>
#include <iostream>

GrblStreamer::GrblStreamer(SerialPort& port) : serial(port) {}

bool GrblStreamer::streamFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!sendLine(line)) {
            std::cerr << "Failed to send line: " << line << std::endl;
            return false;
        }
    }
    return true;
}

bool GrblStreamer::sendLine(const std::string& line) {
    serial.write(line + "\n");
    std::string response = serial.readLine();
    return response.find("ok") != std::string::npos;
}
