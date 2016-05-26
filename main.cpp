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
    std::string s = "15";
    serial.async_write(s);
    std::string st = "Hi";
    serial.async_write(st);
    serial.async_read_until("\n");
    // const char arr[] = {'2', '5', '5'};
    // serial.async_write(arr);
    // std::string s = "50089q503320232500202";
    // std::vector<char> data(s.begin(), s.end());
    // serial.async_write_buffer(data);
    io.run();
}
