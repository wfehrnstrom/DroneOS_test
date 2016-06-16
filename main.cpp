#ifndef IOSTREAM_H
#include <iostream>
#endif
#ifndef SERIAL_H
#include "serial.h"
#endif
#include <time.h>
#define PORT "/dev/tty.usbserial-DA01OTZ8"
//#define PORT "/dev/tty.usbmodem1411"

int main(){
    std::clock_t t;
    t = std::clock();
    boost::asio::io_service io;
    Serial::Serial serial(PORT, &io, 19200);
    if(!serial.is_open()){
      serial.open(PORT);
    }
    std::string s = "sup dawg\n";
    serial.write(s);
    // serial.read_until('!');
    // serial.write(s);
    serial.read_until('\n');
    serial.write(s);
    serial.read_until('\n');
    t = std::clock() - t;
    std::cout << "Execution time:" << ((float)t)/CLOCKS_PER_SEC << std::endl;
}
