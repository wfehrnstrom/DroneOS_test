#include "test.h"

class Tester{

public:
  Tester(std::string path);
  void read_command_file();
  std::string extract_all_data_from_command_file();
  std::string* read_includes_in_command_file();
  void delete_all_dynamics();
  std::string *_imports;
  // Test generate_tests();
  // void run_all();
private:
  std::ifstream _command_in;
  std::ofstream _code_out;
  std::string _raw_text;
  Test *_tests;
  std::string get_substring_with(std::string start_of_substring);
  std::string* return_block_as_array_of_lines(std::string start_of_substring, std::string block_common_delim);
  std::string* read_defines_in_command_file(); //#define classifies as a preprocessor directive, DEFINE simply replaces all instances of left arg with right arg
  std::string* read_methods_in_command_file();
  std::string* read_params_in_command_file();
  std::string* read_dos_in_command_file();
  std::string *_defines;
  std::string *_methods;
  std::string *_params;
  std::string *_dos;
};
