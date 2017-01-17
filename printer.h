#import <iostream>

class Printer{
public:
  void print(std::string string);
  void concat(std::string string1, std::string string2);
  void print_with_tabs(std::string string);
  void print_num_times(std::string string, int num_times);
};
