#include "grbl_streamer.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // Check command-line arguments
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: " << argv[0] << " <port> [filename]" << std::endl;
        std::cerr << "Example: " << argv[0] << " /dev/ttyUSB0 gcode.nc" << std::endl;
        std::cerr << "         " << argv[0] << " COM3 gcode.nc" << std::endl;
        return 1;
    }

    std::string port = argv[1];
    std::string filename = (argc == 3) ? argv[2] : "";
    int baudrate = 115200; // Default for GRBL

    try {
        // Initialize GRBL streamer
        GrblStreamer streamer(port, baudrate);

        // Set response callback to print GRBL output
        streamer.setResponseCallback([](const std::string& response) {
            std::cout << "GRBL: " << response << std::endl;
        });

        // If a file is provided, load and stream it
        if (!filename.empty()) {
            if (!streamer.loadFile(filename)) {
                std::cerr << "Failed to load file: " << filename << std::endl;
                return 1;
            }
            std::cout << "Streaming " << filename << " to GRBL on " << port << std::endl;
            streamer.stream();
        } else {
            // Example: Send a single command (e.g., status query)
            std::cout << "No file provided, sending status query (?)" << std::endl;
            streamer.sendCommand("?\n");
            std::string response = streamer.readResponse();
            std::cout << "GRBL Response: " << response << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
