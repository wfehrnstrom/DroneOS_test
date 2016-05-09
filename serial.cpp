// #ifndef IOSTREAM_H
// #include <iostream>
// #endif
// #include "serial.h"
// #include <boost/algorithm/string.hpp>
//
// Serial::Serial(std::string port_name, boost::asio::io_service* io):port_name_(), readBuf_(new const boost::asio::mutable_buffer()), writeBuf_(new const boost::asio::mutable_buffer()){
//   //In default constructor, set baud rate to 9600
//   port_ = new boost::asio::serial_port(*io, port_name);
//   port_.set_option(boost::asio::serial_port::baud_rate(9600));
//   //Flow Control: allow the ability to slow down serial data in a wire and even stop it
//   port_.set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::none));
//   //Specify whether to send a bit at the end of each binary transmission indicating whether the number of bits was odd
//   //or even.
//   port_.set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none));
//   //Specify how many stop bits there are at the end of the serial transmission
//   port_.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one));
//   //open the serial port at the device name
//   port_name_ = port_name;
//   openAndWaitOnPort(port_name);
// }
//
// Serial::Serial(std::string port_name, boost::asio::io_service* io, int baud_rate, std::string flow_control, std::string parity, float stop_bits):port_(*io, port_name),port_name_(), readBuf_(), writeBuf_(){
//   port_.set_option(boost::asio::serial_port::baud_rate(baud_rate));
//   //Configure parity and check for all cases
//   //if parity == none
//   if(parity == "none"){
//     port_.set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none));
//   }
//   //if parity is odd
//   else if(parity == "odd"){
//     port_.set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::odd));
//   }
//   //if parity is even
//   else if(parity == "even"){
//     port_.set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::even));
//   }
//
//   //Configure flow control
//   if(flow_control ==  "software"){
//     port_.set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::software));
//   }
//   else if(flow_control=="hardware"){
//     port_.set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::hardware));
//   }
//   else{
//     port_.set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::none));
//   }
//
//   //Configure stop_bits
//   if(stop_bits == 1.0){
//     port_.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one));
//   }
//   else if(stop_bits == 1.5){
//     port_.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::onepointfive));
//   }
//   else{
//     port_.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::two));
//   }
//     openAndWaitOnPort(port_name);
// }
//
// Serial::~Serial(){
//   std::cout << "Serial Port is being deleted" << std::endl;
// }
//
// void Serial::write(std::string in){
//   try{
//     port_.write_some(boost::asio::buffer(in));
//       std::cout<< "Serial Data Written" <<std::endl;
//   }
//   catch(boost::system::system_error e){
//     std::cerr << "Serial Write Operation Failed" << std::endl;
//   }
// }
//
// std::string Serial::read(std::string delimiter){
//   int bufSize = boost::asio::buffer_size(readBuf_);
//   boost::asio::read(port_, readBuf_);
//   //char charBuf[bufSize];
//   //charBuf = boost::asio::buffer_cast<char[]>(readBuf_);
//   std::string read = *boost::asio::buffer_cast<std::string*>(readBuf_);
//   //std::string read(charBuf, bufSize);
//   boost::algorithm::trim(read);
//   return read;
// }
//
// void Serial::open(std::string port_name){
//   port_.open(port_name);
//     boost::asio::io_service io;
//   boost::asio::deadline_timer t(io, boost::posix_time::seconds(2));
//   t.wait();
//     std::cout << "Port opened!" << std::endl;
// }
//
// void Serial::close(){
//   port_.close();
// }
//
// bool Serial::is_open(){
//   if(port_.is_open()){
//     return true;
//   }
//   return false;
// }
//
// void Serial::cancel(){
//   port_.cancel();
// }
//
// void Serial::wait(int time){
//     boost::asio::io_service io;
//     boost::asio::deadline_timer t(io, boost::posix_time::seconds(time));
//     t.wait();
// }
//
// void Serial::openAndWaitOnPort(std::string port_name){
//     if(!this->is_open()){
//         port_.open(port_name);
//     }
//     else{
//         this->wait(2);
//         std::cout<< "Done waiting." <<std::endl;
//     }
// }
//
// void Serial::async_read(){
//   boost::system::error_code e;
//   boost::asio::async_read(port_, readBuf_, boost::bind(&Serial::async_read_handler, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
// }
//
// void Serial::async_read_handler(boost::system::error_code &e, std::size_t bytes_read){
//   if(!e){
//     if(bytes_read <= 0){
//       std::cout << "Serial Port with ID: " << port_name_ << " did not read any bytes from the stream." << std::endl;
//     }
//   }
//   else{
//     std::cerr << e.message() << std::endl;
//   }
// }
//NEED MAIN IN ORDER TO NOT THROW ERROR:Undefined symbols for architecture x86_64:
//   "_main", referenced from:
//      implicit entry/start for main executable
// ld: symbol(s) not found for architecture x86_64
// clang: error: linker command failed with exit code 1 (use -v to see invocation)
// make: *** [all] Error 1
#ifndef IOSTREAM_H
#include <iostream>
#endif
#include "serial.h"
#include <boost/algorithm/string.hpp>

Serial::Serial(std::string port_name, boost::asio::io_service* io):port_name_(){
  //In default constructor, set baud rate to 9600
  port_ = new boost::asio::serial_port(*io, port_name);
  boost::asio::mutable_buffer* mutableReadBuf = new boost::asio::mutable_buffer();
  boost::asio::mutable_buffer* mutableWriteBuf = new boost::asio::mutable_buffer();
  //b_ = new boost::asio::streambuf(1024, alloc);
  readBuf_ = new boost::asio::mutable_buffers_1(*mutableReadBuf);
  writeBuf_ = new boost::asio::mutable_buffers_1(*mutableWriteBuf);
  port_->set_option(boost::asio::serial_port::baud_rate(9600));
  //Flow Control: allow the ability to slow down serial data in a wire and even stop it
  port_->set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::none));
  //Specify whether to send a bit at the end of each binary transmission indicating whether the number of bits was odd
  //or even.
  port_->set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none));
  //Specify how many stop bits there are at the end of the serial transmission
  port_->set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one));
  //open the serial port at the device name
  port_name_ = port_name;
  openAndWaitOnPort(port_name);
}

Serial::Serial(std::string port_name, boost::asio::io_service* io, int baud_rate, std::string flow_control, std::string parity, float stop_bits):port_name_(){
  port_ = new boost::asio::serial_port(*io, port_name);
  boost::asio::mutable_buffer* mutableReadBuf = new boost::asio::mutable_buffer();
  boost::asio::mutable_buffer* mutableWriteBuf = new boost::asio::mutable_buffer();
  readBuf_ = new boost::asio::mutable_buffers_1(*mutableReadBuf);
  writeBuf_ = new boost::asio::mutable_buffers_1(*mutableWriteBuf);
  port_->set_option(boost::asio::serial_port::baud_rate(baud_rate));
  //Configure parity and check for all cases
  //if parity == none
  if(parity == "none"){
    port_->set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none));
  }
  //if parity is odd
  else if(parity == "odd"){
    port_->set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::odd));
  }
  //if parity is even
  else if(parity == "even"){
    port_->set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::even));
  }

  //Configure flow control
  if(flow_control ==  "software"){
    port_->set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::software));
  }
  else if(flow_control=="hardware"){
    port_->set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::hardware));
  }
  else{
    port_->set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::none));
  }

  //Configure stop_bits
  if(stop_bits == 1.0){
    port_->set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one));
  }
  else if(stop_bits == 1.5){
    port_->set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::onepointfive));
  }
  else{
    port_->set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::two));
  }
    openAndWaitOnPort(port_name);
}

Serial::~Serial(){
  std::cout << "Serial Port is being deleted" << std::endl;
}

void Serial::write(std::string in){
  try{
    port_->write_some(boost::asio::buffer(in));
      std::cout<< "Serial Data Written" <<std::endl;
  }
  catch(boost::system::system_error e){
    std::cerr << "Serial Write Operation Failed" << std::endl;
  }
}

std::string Serial::read(std::string delimiter){
  int bufSize = boost::asio::buffer_size(*readBuf_);
  boost::asio::read(*port_, *readBuf_);
  //char charBuf[bufSize];
  //charBuf = boost::asio::buffer_cast<char[]>(readBuf_);
  std::string read = *boost::asio::buffer_cast<std::string*>(*readBuf_);
  //std::string read(charBuf, bufSize);
  boost::algorithm::trim(read);
  return read;
}

void Serial::open(std::string port_name){
  port_->open(port_name);
    boost::asio::io_service io;
  boost::asio::deadline_timer t(io, boost::posix_time::seconds(2));
  t.wait();
    std::cout << "Port opened!" << std::endl;
}

void Serial::close(){
  port_->close();
}

bool Serial::is_open(){
  if(port_->is_open()){
    return true;
  }
  return false;
}

void Serial::cancel(){
  port_->cancel();
}

void Serial::wait(int time){
    boost::asio::io_service io;
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(time));
    t.wait();
}

void Serial::openAndWaitOnPort(std::string port_name){
    if(!this->is_open()){
        port_->open(port_name);
    }
    else{
        this->wait(2);
        std::cout<< "Done waiting." <<std::endl;
    }
}

void Serial::async_read(){
  boost::system::error_code e;
  boost::asio::async_read(*port_, *readBuf_, boost::bind(&Serial::async_read_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Serial::async_read_handler(const boost::system::error_code &e, std::size_t bytes_read){
  std::cout << "Entered serial read handler" << std::endl;
  if(!(*e)){
    //std::cout << e.message() << std::endl;
    std::cout << bytes_read << std::endl;
    if(bytes_read > 0){
      std::string read = *boost::asio::buffer_cast<std::string*>(*readBuf_);
      std::cout << "Read: " <<std::endl;
      std::cout << read <<std::endl;
      //boost::asio::async_read(*port_, *readBuf_, boost::bind(&Serial::async_read_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    else{
      std::cout << readBuf_ <<std::endl;
    }
  }
  else{
    std::cout << "Error occurred!" << std::endl;
    std::cerr << e.message() << std::endl;
  }
}
