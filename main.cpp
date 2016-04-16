#ifndef IOSTREAM_H
#include <iostream>
#endif
//HI GIT
#ifndef SERIAL_H
#include "serial.h"
#endif

//#include <boost/asio.hpp>
#define PORT "/dev/tty.usbmodem1411"

int main(){
    boost::asio::io_service io;
    Serial::Serial serial(PORT, &io);
    if(!serial.is_open()){
    serial.open(PORT);
    }
    serial.write("255");
    serial.async_read();
    io.run();
    //serial.close();
    return 0;
}
