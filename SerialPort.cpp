// serial.cpp
#include "serial.h"
#include <stdexcept>
#include <string>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#endif

Serial::Serial(const std::string& port, int baudrate) : port_(port), baudrate_(baudrate), fd_(-1) {
#ifdef _WIN32
    std::string portName = "\\\\.\\" + port; // Windows requires "\\.\COMX" format
    hSerial_ = CreateFileA(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
                           OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSerial_ == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Failed to open serial port: " + port);
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial_, &dcbSerialParams)) {
        CloseHandle(hSerial_);
        throw std::runtime_error("Failed to get serial port state");
    }

    dcbSerialParams.BaudRate = baudrate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hSerial_, &dcbSerialParams)) {
        CloseHandle(hSerial_);
        throw std::runtime_error("Failed to set serial port state");
    }

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if (!SetCommTimeouts(hSerial_, &timeouts)) {
        CloseHandle(hSerial_);
        throw std::runtime_error("Failed to set serial timeouts");
    }
#else
    fd_ = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd_ == -1) {
        throw std::runtime_error("Failed to open serial port: " + port + " (" + std::strerror(errno) + ")");
    }

    struct termios tty;
    if (tcgetattr(fd_, &tty) != 0) {
        close(fd_);
        throw std::runtime_error("Failed to get serial port attributes");
    }

    cfsetospeed(&tty, baudrate);
    cfsetispeed(&tty, baudrate);

    tty.c_cflag &= ~PARENB; // No parity
    tty.c_cflag &= ~CSTOPB; // One stop bit
    tty.c_cflag &= ~CSIZE;  // Clear size bits
    tty.c_cflag |= CS8;     // 8 bits
    tty.c_cflag &= ~CRTSCTS; // Disable hardware flow control
    tty.c_cflag |= CREAD | CLOCAL; // Enable receiver, ignore modem status

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Raw mode
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Disable software flow control
    tty.c_oflag &= ~OPOST; // Raw output

    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout

    if (tcsetattr(fd_, TCSANOW, &tty) != 0) {
        close(fd_);
        throw std::runtime_error("Failed to set serial port attributes");
    }
#endif
}

Serial::~Serial() {
#ifdef _WIN32
    if (hSerial_ != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial_);
    }
#else
    if (fd_ != -1) {
        close(fd_);
    }
#endif
}

bool Serial::write(const std::string& data) {
#ifdef _WIN32
    DWORD bytesWritten;
    if (!WriteFile(hSerial_, data.c_str(), data.size(), &bytesWritten, NULL)) {
        return false;
    }
    return bytesWritten == data.size();
#else
    ssize_t bytesWritten = ::write(fd_, data.c_str(), data.size());
    return bytesWritten == static_cast<ssize_t>(data.size());
#endif
}

std::string Serial::read() {
    std::string result;
#ifdef _WIN32
    char buffer[128];
    DWORD bytesRead;
    if (ReadFile(hSerial_, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
        buffer[bytesRead] = '\0';
        result = buffer;
    }
#else
    char buffer[128];
    ssize_t bytesRead = ::read(fd_, buffer, sizeof(buffer) - 1);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        result = buffer;
    }
#endif
    return result;
}
