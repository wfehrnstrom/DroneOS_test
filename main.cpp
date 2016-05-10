#ifndef IOSTREAM_H
#include <iostream>
#endif
#ifndef SERIAL_H
#include "serial.h"
#endif
#define PORT "/dev/tty.usbmodem1411"

int main(){
    boost::asio::io_service io;
    Serial::Serial serial(PORT, &io, 9600);
    if(!serial.is_open()){
    serial.open(PORT);
    }
    //serial.write("255");
    //serial.async_read_until("\n");
    // const char arr[] = {'2', '5', '5'};
    // serial.async_write(arr);
    serial.async_write("255");
    io.run();
    return 0;
}
