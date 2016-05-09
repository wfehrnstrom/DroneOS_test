#ifndef IOSTREAM_H
#include <iostream>
#endif
#ifndef SERIAL_H
#include "serial.h"
#endif
#define PORT "/dev/tty.usbmodem1411"

// void async_read_handler(const boost::system::error_code &e, std::size_t bytes_read){
//   if(!e){
//     std::cout << e.message() << std::endl;
//     std::cout << bytes_read << std::endl;
//     if(bytes_read <= 0){
//       boost::asio::async_read(*port, boost::asio::buffer(*buf), boost::bind(async_read_handler, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
//     }
//     else{
//       std::cout << "Bytes read: " << bytes_read << std::endl;
//     }
//   }
//   else{
//     std::cerr << e.message() << std::endl;
//   }
// }

int main(){
    boost::asio::io_service io;
    Serial::Serial serial(PORT, &io);
    if(!serial.is_open()){
    serial.open(PORT);
    }
    io.run();
    serial.async_read();
    return 0;
}
