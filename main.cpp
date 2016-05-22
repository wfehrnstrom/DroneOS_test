#ifndef IOSTREAM_H
#include <iostream>
#endif
#ifndef SERIAL_H
#include "serial.h"
#endif
#define PORT "/dev/tty.usbmodem1411"

std::string printStream(std::ostream& os){
  std::ostringstream oss;
  oss << os;
  std::string dataInBuf = oss.str();
  return dataInBuf;
}

int main(){
    // boost::asio::io_service io;
    // Serial::Serial serial(PORT, &io, 9600);
    // if(!serial.is_open()){
    // serial.open(PORT);
    // }
    // //serial.write("255");
    // //serial.async_read_until("\n");
    // // const char arr[] = {'2', '5', '5'};
    // // serial.async_write(arr);
    // std::string s = "50";
    // serial.async_write(s);
    // io.run();
    typedef std::allocator<char> Allocator;
    Allocator* alloc = new Allocator();
    boost::asio::streambuf* b;
    b = new boost::asio::streambuf(1024, *alloc);
    std::ostream out(b);
    out << "hi";
    std::cout << out.tellp() << std::endl;
    return 0;
}
