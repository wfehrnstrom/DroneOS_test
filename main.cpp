#ifndef IOSTREAM_H
#include <iostream>
#endif
#ifndef SERIAL_H
#include "serial.h"
#endif
#ifndef TESTER_H
#include "tester.h"
#endif
#ifndef PRINTER_H
#include "printer.h"
#endif
#define SERIALPORT "/dev/tty.usbserial-DA01OTZ8"
//#define SERIALPORT "/dev/tty.usbmodem1411"

int main(){
  // boost::asio::io_service io;
  // Serial serial(SERIALPORT, &io, 19200);
  // std::string string = "sup dawg\n";
  // serial.queue_in_streambuf(string);
  // serial.write_all_data_in_streambuf();
  // serial.async_read_all_incoming_data();
  // io.run();
  std::string path = "test.txt";
  Tester tester(path);
  tester.read_command_file();
  tester.delete_all_dynamics();
}
