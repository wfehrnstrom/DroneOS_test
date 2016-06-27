#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#ifndef SERIAL_H
#define SERIAL_H
#endif

class Serial{

public:
  enum flow_control{no_flow_control, software, hardware};
  enum parity{no_parity, odd, even};
  enum stop_bit{one, one_and_a_half, two};
  Serial(std::string port_name, boost::asio::io_service* io, std::size_t baud_rate, flow_control serial_flow_control = no_flow_control, parity serial_parity = no_parity, stop_bit stop_bits = one);
  ~Serial();
  void write(std::string stringToBuffer);
  void write_all_data_in_streambuf();
  void cancel();
  void open(std::string port_name);
  void close();
  void wait(int time);
  bool is_open();
  void async_read_handler(const boost::system::error_code &e, std::size_t bytes_read);
  void async_write_handler(const boost::system::error_code &e, std::size_t bytes_read);
  void async_write(std::string outgoing_string);
  void async_write_exactly(int num_bytes);
  void async_write_all_data_in_streambuf();
  void async_read_until(char delim);
  void async_read_all_incoming_data();
  std::size_t read_until(char delimiter);
  std::size_t read_at_least(std::size_t num_bytes);
  std::string read_until_and_retrieve(char delimiter);
  std::string read_and_retrieve_at_least(std::size_t num_bytes);
  std::string retrieve_data_from_streambuf();
  void queue_in_streambuf(std::string outgoing_string);
  //Setters, Getters, and Boolean state checkers
  boost::asio::serial_port* get_port();
  bool diagnostics_enabled();
  void set_diagnostics_enabled_to(bool state);
private:
  boost::asio::streambuf* b_;
  void set_port(boost::asio::serial_port* port);
  void configure_baud_rate(std::size_t baud_rate);
  void configure_parity(parity serial_parity);
  void configure_flow_control(flow_control serial_flow_control);
  void configure_stop_bits(stop_bit num_stop_bits);
  void log_error_and_log_message(const boost::system::error_code &e, std::string custom_error_message);
  void print_read_diagnostics();
  void print_write_diagnostics();
  std::string port_name_;
  boost::asio::serial_port* port_;
  bool _diagnostics_enabled;
};
