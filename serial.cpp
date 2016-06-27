#ifndef IOSTREAM_H
#include <iostream>
#endif
#include "serial.h"
#include <boost/algorithm/string.hpp>
#include <boost/thread.hpp>
#include <fstream>
#define TOTAL_BYTES_IN_STREAMBUF 1024
#define SECONDS_TO_WAIT_ON_OPEN 2

Serial::Serial(std::string port_name, boost::asio::io_service* io, std::size_t baud_rate, flow_control serial_flow_control, parity serial_parity, stop_bit stop_bits):port_name_(){
  this->set_port(new boost::asio::serial_port(*io, port_name));
  std::allocator<char>* alloc = new std::allocator<char>();
  b_ = new boost::asio::streambuf(TOTAL_BYTES_IN_STREAMBUF, *alloc);
  configure_baud_rate(baud_rate);
  configure_parity(serial_parity);
  configure_flow_control(serial_flow_control);
  configure_stop_bits(stop_bits);
  open(port_name);
}

Serial::~Serial(){
  this->close();
  std::cout << "Serial Port deleted." << std::endl;
}

bool Serial::diagnostics_enabled(){
  return _diagnostics_enabled;
}

void Serial::set_diagnostics_enabled_to(bool state){
  _diagnostics_enabled = state;
}

void Serial::log_error_and_log_message(const boost::system::error_code &e, std::string custom_error_message){
  std::cerr << custom_error_message << std::endl;
  std::cerr << e.message() << std::endl;
}

boost::asio::serial_port* Serial::get_port(){
  return this->port_;
}

void Serial::set_port(boost::asio::serial_port* port){
  this->port_ = port;
}

void Serial::configure_baud_rate(std::size_t baud_rate){
  this->get_port()->set_option(boost::asio::serial_port::baud_rate(baud_rate));
}

void Serial::configure_parity(parity serial_parity){
  //Configure parity and check for all cases
  if(serial_parity == no_parity){
    this->get_port()->set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none));
  }
  else if(serial_parity == odd){
    this->get_port()->set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::odd));
  }
  else if(serial_parity == even){
    this->get_port()->set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::even));
  }
}

void Serial::configure_flow_control(flow_control serial_flow_control){
  if(serial_flow_control == software){
    this->get_port()->set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::software));
  }
  else if(serial_flow_control == hardware){
    this->get_port()->set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::hardware));
  }
  else if(serial_flow_control == no_flow_control){
    this->get_port()->set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::none));
  }
}

void Serial::configure_stop_bits(stop_bit stop_bits){
  if(stop_bits == one){
    this->get_port()->set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one));
  }
  else if(stop_bits == one_and_a_half){
    this->get_port()->set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::onepointfive));
  }
  else if(stop_bits == two){
    this->get_port()->set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::two));
  }
}

std::size_t Serial::read_until(char delimiter){
  std::size_t bytes_read;
  try{
    bytes_read = boost::asio::read_until(*port_, *b_, delimiter);
  }catch(boost::system::error_code e){
    log_error_and_log_message(e, "Synchronous read until failed.");
  }
  return bytes_read;
}

std::string Serial::read_until_and_retrieve(char delimiter){
  std::string read = "";
  try{
    //bytes_read consists of a string, and then a carriage return ascii value with a newline ascii value
    std::size_t bytes_read = boost::asio::read_until(*port_, *b_, delimiter);
    read = retrieve_data_from_streambuf();
  }
  catch(boost::system::error_code e){
    log_error_and_log_message(e,  "Serial synchronous read_until_and_retrieve operation failed.");
  }
  return read;
}

std::size_t Serial::read_at_least(std::size_t num_bytes){
  std::size_t bytes_read = boost::asio::read(*port_, *b_, boost::asio::transfer_at_least(num_bytes));
  return bytes_read;
}

std::string Serial::read_and_retrieve_at_least(std::size_t num_bytes){
  std::size_t bytes_read = boost::asio::read(*port_, *b_, boost::asio::transfer_at_least(num_bytes));
  std::string read = retrieve_data_from_streambuf();
  return read;
}

std::string Serial::retrieve_data_from_streambuf(){
  std::string read = "";
  try{
    std::istream* instream = new std::istream(b_);
    std::getline(*instream, read);
    boost::algorithm::trim(read);
  }catch(boost::system::error_code e){
    log_error_and_log_message(e, "data retrieval from streambuf was unsuccessful.");
  }
  return read;
}

void Serial::open(std::string port_name){
  if(!port_->is_open()){
    port_->open(port_name);
    boost::asio::io_service io;
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(SECONDS_TO_WAIT_ON_OPEN));
    t.wait();
  }
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


void Serial::write(std::string in){
  try{
    port_->write_some(boost::asio::buffer(in));
  }
  catch(boost::system::error_code e){
    log_error_and_log_message(e,  "Serial synchronous write operation failed.");
  }
}

void Serial::write_all_data_in_streambuf(){
  try{port_->write_some(b_->data());}
  catch(boost::system::error_code e){
    log_error_and_log_message(e,  "Total write of everything in streambuf was unsuccessful.");
  }
}

void Serial::async_write_handler(const boost::system::error_code &e, std::size_t bytes_written){
  if(!e){
    b_->consume(bytes_written);
  }else{
    log_error_and_log_message(e,  "Consuming of bytes in member streambuf failed.");
  }
}

void Serial::async_write_exactly(int num_bytes){
  boost::asio::async_write(*port_, b_->prepare(num_bytes), boost::bind(&Serial::async_write_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Serial::async_write(std::string string){
  std::string data(string);
  boost::asio::async_write(*port_, boost::asio::buffer(data), boost::bind(&Serial::async_write_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Serial::async_write_all_data_in_streambuf(){
  boost::asio::async_write(*port_, *b_, boost::bind(&Serial::async_write_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Serial::queue_in_streambuf(std::string outgoing_string){
  std::ostream* outstream = new std::ostream(b_);
  (*outstream) << outgoing_string;
}

void Serial::print_write_diagnostics(){
  
}

void Serial::async_read_handler(const boost::system::error_code &e, std::size_t bytes_read){
  if(!(e)){
    std::cout << "Bytes read in async read handler:" << bytes_read << std::endl;
    if(diagnostics_enabled()){

    }
  }
  else{
    log_error_and_log_message(e, "Generic asynchronous read error.");
  }
}

void Serial::async_read_until(char delim){
  boost::asio::async_read_until(*port_, *b_, delim, boost::bind(&Serial::async_read_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Serial::async_read_all_incoming_data(){
  boost::asio::async_read(*port_, *b_, boost::bind(&Serial::async_read_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void print_read_diagnostics(){
  std::cout << "DIAGNOSTICS BEGIN" << std::endl;
  //TODO peek in buffer to get data in out buffer without removing it from that buffer
  std::cout << "DIAGNOSTICS END" << std::endl;
}
