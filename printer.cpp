#include <iostream>
#include "printer.h"

void Printer::print(std::string string){
  std::cout << string << std::endl;
}

void Printer::concat(std::string string1, std::string string2){
  std::cout << string1 << string2 << std::endl;
}

void Printer::print_with_tabs(std::string string){
  std::cout << "\t\t" << string << std::endl;
}

void Printer::print_num_times(std::string string, int num_times){
  for(int i = 0; i < num_times; i++){
    std::cout << string << " ";
  }
  std::cout << "\n";
}
