CXX = g++
CXXFLAGS = -std=c++11 -Wall

OBJS = main.o GrblStreamer.o SerialPort.o

all: grbl_test

grbl_test: $(OBJS)
	$(CXX) $(CXXFLAGS) -o grbl_test $(OBJS)

main.o: main.cpp GrblStreamer.h SerialPort.h
GrblStreamer.o: GrblStreamer.cpp GrblStreamer.h SerialPort.h
SerialPort.o: SerialPort.cpp SerialPort.h

clean:
	 rm -f *.o grbl_test
