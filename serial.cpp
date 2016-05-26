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
// Serial::Serial(std::string port_name, boost::asio::io_service* io):port_name_(){
//   //In default constructor, set baud rate to 9600
//   port_ = new boost::asio::serial_port(*io, port_name);
//   unsigned char data[] = {};
//   boost::asio::mutable_buffer* mutableReadBuf = new boost::asio::mutable_buffer(data,2);
//   boost::asio::mutable_buffer* mutableWriteBuf = new boost::asio::mutable_buffer();
//   typedef std::allocator<char> Allocator;
//   Allocator* alloc = new Allocator();
//   b_ = new boost::asio::streambuf(1024, *alloc);
//   readBuf_ = new boost::asio::mutable_buffers_1(*mutableReadBuf);
//   writeBuf_ = new boost::asio::mutable_buffers_1(*mutableWriteBuf);
//   port_->set_option(boost::asio::serial_port::baud_rate(9600));
//   //Flow Control: allow the ability to slow down serial data in a wire and even stop it
//   port_->set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::none));
//   //Specify whether to send a bit at the end of each binary transmission indicating whether the number of bits was odd
//   //or even.
//   port_->set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none));
//   //Specify how many stop bits there are at the end of the serial transmission
//   port_->set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one));
//   //open the serial port at the device name
//   port_name_ = port_name;
//   openAndWaitOnPort(port_name);
// }
//
// Serial::Serial(std::string port_name, boost::asio::io_service* io, int baud_rate):port_name_(){
//   //In default constructor, set baud rate to 9600
//   port_ = new boost::asio::serial_port(*io, port_name);
//   unsigned char data[] = {};
//   boost::asio::mutable_buffer* mutableReadBuf = new boost::asio::mutable_buffer(data,2);
//   boost::asio::mutable_buffer* mutableWriteBuf = new boost::asio::mutable_buffer();
//   typedef std::allocator<char> Allocator;
//   Allocator* alloc = new Allocator();
//   b_ = new boost::asio::streambuf(1024, *alloc);
//   readBuf_ = new boost::asio::mutable_buffers_1(*mutableReadBuf);
//   writeBuf_ = new boost::asio::mutable_buffers_1(*mutableWriteBuf);
//   port_->set_option(boost::asio::serial_port::baud_rate(baud_rate));
//   //Flow Control: allow the ability to slow down serial data in a wire and even stop it
//   port_->set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::none));
//   //Specify whether to send a bit at the end of each binary transmission indicating whether the number of bits was odd
//   //or even.
//   port_->set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none));
//   //Specify how many stop bits there are at the end of the serial transmission
//   port_->set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one));
//   //open the serial port at the device name
//   port_name_ = port_name;
//   openAndWaitOnPort(port_name);
// }

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
    }
}

void Serial::async_write_handler(const boost::system::error_code &e, std::size_t bytes_written){
  std::cout << "Data written" << std::endl;
  b_->consume(bytes_written);
}

void Serial::async_write(int n){
  boost::asio::async_write(*port_, b_->prepare(n), boost::bind(&Serial::async_write_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

std::string printStream(std::ostream& os){
  std::ostringstream oss;
  oss << os;
  std::string dataInBuf = oss.str();
  return dataInBuf;
}

void Serial::async_write(const char data[]){
  std::string str(data);
  std::ofstream outfile;
  outfile.open("test.txt");
  std::ostream out(b_);
  std::cout << "PRINT STREAM1:" << printStream(out) << std::endl;
  outfile << str;
  outfile << str;
  std::cout << "data:" << str << std::endl;
  // boost::this_thread::sleep(boost::posix_time::seconds(2));
  out.put('a');
  out.put('b');
  out.put('c');
  std::cout << "PS0:" << printStream(outfile) << std::endl;
  out.flush();
  out.seekp(0);
  std::cout << "SEEK P 0:" << printStream(out) << std::endl;
  out.seekp(1);
  std::cout << "PS:" << printStream(out) << std::endl;
  std::cout << "SEEK P 1:" << out.tellp() << std::endl;

  boost::asio::async_write(*port_, b_->prepare(str.length()), boost::bind(&Serial::async_write_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Serial::async_write(std::string string){
  std::cout << "String size:" << string.size() << std::endl;
  // char stringToChar[string.size() + 1];
  // strcpy(stringToChar, string.c_str());
  // this->async_write(stringToChar);
  boost::asio::async_write(*port_, boost::asio::buffer(string, string.length()), boost::bind(&Serial::async_write_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Serial::async_write_buffer(std::vector<char> data){
  int num = data.size();
  std::cout << num << std::endl;
  boost::asio::mutable_buffer buf(&data, data.size());
  boost::asio::async_write(*port_, boost::asio::buffer(data, data.size()), boost::bind(&Serial::async_write_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Serial::async_read_handler(const boost::system::error_code &e, std::size_t bytes_read){
  if(!(*e)){
    std::cout << "bytes read in async read handler:" << bytes_read << std::endl;
    if(bytes_read > 0){
      b_->commit(bytes_read);
      std::istream* instream = new std::istream(b_);
      std::ostream* outstream = new std::ostream(b_);
      std::string outstreamtostring;
      std::string streamtostring;
      *outstream << outstreamtostring;
      *instream >> streamtostring;
      std::cout << "Read: " <<std::endl << streamtostring <<std::endl;
      std::cout << "Read from outbuf:" << std::endl <<  outstreamtostring << std::endl;
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
