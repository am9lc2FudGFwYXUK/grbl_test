# grbl_test

This repository contains a simple GRBL streaming example in C++ using a stub serial port class.  
Replace the serial port implementation with platform-specific code for a real application.

## Files
- `GrblStreamer.h/cpp`: Main GRBL streaming logic
- `SerialPort.h/cpp`: Stub serial port interface (replace with actual implementation)
- `main.cpp`: Example usage
- `Makefile`: Build instructions

## Build
```sh
make
```