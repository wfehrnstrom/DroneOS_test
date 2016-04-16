#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#ifndef SERIAL_H
#define SERIAL_H
class Serial{
public:
  Serial(std::string port_name,boost::asio::io_service* io, int baud_rate, std::string flow_control, std::string parity, float stop_bits);
  Serial(std::string port_name, boost::asio::io_service* io);
  ~Serial();
  void write(std::string stringToBuffer);
  std::string read(std::string delimiter);
  void cancel();
  void open(std::string port_name);
  void openAndWaitOnPort(std::string port_name);
  void close();
  void wait(int time);
  bool is_open();
  void async_read();
  void async_read_handler(boost::system::error_code &e, std::size_t bytes_read);
//boost::asio::serial_port &port, boost::asio::mutable_buffer &readBuffer, bool (Serial::*)(boost::system::error_code &e, std::size_t bytes_read)
private:
  std::string port_name_;
  boost::asio::serial_port* port_ ;
  //boost::asio::mutable_buffers_1 readBuf_(const boost::asio::mutable_buffer buf());
  boost::asio::mutable_buffers_1* readBuf_;
  boost::asio::mutable_buffers_1* writeBuf_;
};
#endif
