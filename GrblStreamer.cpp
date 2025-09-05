// grbl_streamer.cpp
#include "grbl_streamer.h"
#include <stdexcept>
#include <algorithm>

GrblStreamer::GrblStreamer(const std::string& port, int baudrate)
    : serial_(port, baudrate), bufferAvailable_(128) {}

GrblStreamer::~GrblStreamer() {}

bool GrblStreamer::loadFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Remove comments and whitespace
        size_t commentPos = line.find(';');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        if (!line.empty()) {
            commandQueue_.push(line + "\n");
        }
    }
    file.close();
    return true;
}

void GrblStreamer::stream() {
    while (!commandQueue_.empty()) {
        std::string command = commandQueue_.front();
        int commandSize = calculateBufferUsage(command);

        if (commandSize <= bufferAvailable_) {
            if (sendCommand(command)) {
                commandQueue_.pop();
                bufferAvailable_ -= commandSize;
            } else {
                break; // Handle serial write failure
            }
        }

        // Read and process GRBL responses
        std::string response = serial_.read();
        if (!response.empty()) {
            processResponse(response);
        }
    }
}

bool GrblStreamer::sendCommand(const std::string& command) {
    return serial_.write(command);
}

void GrblStreamer::processResponse(const std::string& response) {
    if (responseCallback_) {
        responseCallback_(response);
    }
    if (response.find("ok") != std::string::npos) {
        bufferAvailable_ = 128; // Reset buffer on "ok" (simplified for demo)
    } else if (response.find("error") != std::string::npos) {
        // Handle error (e.g., log or stop streaming)
        if (responseCallback_) {
            responseCallback_("Error received: " + response);
        }
    }
}

int GrblStreamer::calculateBufferUsage(const std::string& command) {
    // GRBL counts characters excluding the newline
    return command.size() - 1;
}

void GrblStreamer::setResponseCallback(std::function<void(const std::string&)> callback) {
    responseCallback_ = callback;
}
