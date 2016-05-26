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
#include <boost/thread.hpp>
#include <fstream>

Serial::Serial(std::string port_name, boost::asio::io_service* io, int baud_rate, std::string flow_control, std::string parity, float stop_bits):port_name_(){
  port_ = new boost::asio::serial_port(*io, port_name);
  boost::asio::mutable_buffer* mutableReadBuf = new boost::asio::mutable_buffer();
  boost::asio::mutable_buffer* mutableWriteBuf = new boost::asio::mutable_buffer();
  readBuf_ = new boost::asio::mutable_buffers_1(*mutableReadBuf);
  writeBuf_ = new boost::asio::mutable_buffers_1(*mutableWriteBuf);
  typedef std::allocator<char> Allocator;
  Allocator* alloc = new Allocator();
  b_ = new boost::asio::streambuf(1024, *alloc);
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
  this->close();
  std::cout << "Serial Port is being deleted" << std::endl;
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
    }
}

void Serial::async_write_handler(const boost::system::error_code &e, std::size_t bytes_written){
  std::cout << "Data written" << std::endl;
  b_->consume(bytes_written);
}

void Serial::async_write(std::string string){
  std::cout << "String size:" << string.size() << std::endl;
  boost::asio::async_write(*port_, boost::asio::buffer(string, string.length()), boost::bind(&Serial::async_write_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Serial::async_read_handler(const boost::system::error_code &e, std::size_t bytes_read){
  if(!(e)){
    std::cout << "bytes read in async read handler:" << bytes_read << std::endl;
    if(bytes_read > 0){
      //b_->commit(bytes_read);
      std::istream* instream = new std::istream(b_);
      std::ostream* outstream = new std::ostream(b_);
      std::string streamtostring;
      *instream >> streamtostring;
      std::cout << "Read: " <<std::endl << streamtostring <<std::endl;
    }
    else{
      std::cout << "No bytes read" << std::endl;
    }
  }
  else{
    std::cout << "Error occurred!" << std::endl;
    std::cerr << e.message() << std::endl;
  }
}
void Serial::async_read_until(std::string delim){
  boost::system::error_code e;
  boost::asio::async_read_until(*port_, *b_, delim, boost::bind(&Serial::async_read_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}
