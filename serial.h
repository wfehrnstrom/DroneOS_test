#include <boost/asio.hpp>
#include <iostream>
#ifndef SERIAL_H
#define SERIAL_H
class Serial{
public:
  Serial(std::string port_name,boost::asio::io_service* io, int baud_rate, std::string flow_control, std::string parity, float stop_bits);
  Serial(std::string port_name, boost::asio::io_service* io);
  ~Serial();
  void write(std::string stringToBuffer);
  std::string read(int inputBufferLength, std::string delimiter);
  void cancel();
  void open(std::string port_name);
    void openAndClearPort(std::string port_name);
  void close();
  void wait(int time);
  bool is_open();

private:
  boost::asio::serial_port port_;
};
#endif
