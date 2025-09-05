// grbl_streamer.h
#ifndef GRBL_STREAMER_H
#define GRBL_STREAMER_H

#include "serial.h"
#include <queue>
#include <string>
#include <fstream>
#include <functional>

class GrblStreamer {
public:
    GrblStreamer(const std::string& port, int baudrate);
    ~GrblStreamer();

    // Load G-code or commands from a file
    bool loadFile(const std::string& filename);

    // Stream commands to GRBL, respecting the 128-byte buffer
    void stream();

    // Register a callback for handling GRBL responses
    void setResponseCallback(std::function<void(const std::string&)> callback);

private:
    Serial serial_;
    std::queue<std::string> commandQueue_;
    int bufferAvailable_; // Tracks remaining space in GRBL's 128-byte buffer
    std::function<void(const std::string&)> responseCallback_;

    // Parse and send a single command, updating buffer state
    bool sendCommand(const std::string& command);

    // Process GRBL response (e.g., "ok" or error)
    void processResponse(const std::string& response);

    // Calculate buffer usage for a command (excluding \n)
    int calculateBufferUsage(const std::string& command);
};

#endif
