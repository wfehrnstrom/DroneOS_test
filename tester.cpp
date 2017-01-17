#include "tester.h"
#include <iostream>
#define START_OF_FILE_INDEX 0
#define NUM_NEWLINES_IN_PADDING 1

Tester::Tester(std::string path):_command_in(), _code_out(), _raw_text(){
  _command_in.open("test.txt");
  //std::cout << _raw_text << std::endl;
}

void Tester::delete_all_dynamics(){
  delete[] _imports;
  delete[] _defines;
  delete[] _methods;
  delete[] _params;
  delete[] _dos;
}

void Tester::read_command_file(){
  extract_all_data_from_command_file();
  _imports = read_includes_in_command_file();
  _defines = read_defines_in_command_file();
  _methods = read_methods_in_command_file();
  _params = read_params_in_command_file();
  _dos = read_dos_in_command_file();
}

std::string Tester::extract_all_data_from_command_file(){
  while(!_command_in.eof()){
    std::string line;
    std::getline(_command_in, line);
    _raw_text += line;
    _raw_text += "\n";
  }
  std::string start_sequence = "START{";
  std::string end_sequence = "}END";
  std::size_t index_of_start = _raw_text.find(start_sequence);
  _raw_text.replace(0, index_of_start + start_sequence.length(), "");
  std::size_t index_of_end = _raw_text.find(end_sequence);
  std::size_t length_between_end_of_file_and_end = _raw_text.length() - index_of_end;
  _raw_text.replace(index_of_end, length_between_end_of_file_and_end, "");
  return _raw_text;
}

std::string Tester::get_substring_with(std::string start_of_substring){
  std::size_t substring_start_index = _raw_text.find(start_of_substring);
  std::size_t substring_end_index = _raw_text.find("\n\n", substring_start_index);
  return _raw_text.substr(substring_start_index, substring_end_index - substring_start_index);
}

std::string get_line(std::string string, int start_index){
  std::string line = string.substr(start_index, (string.find("\n", start_index) - start_index));
  return line;
}

std::size_t get_num_occurrences_of_substr_in_str(std::string substring, std::string string){
  std::size_t count = 0;
  for(int i = 0; i != -1;){
    i = string.find(substring, i + 1);
    count++;
  }
  return count;
}

std::string* Tester::return_block_as_array_of_lines(std::string block_start, std::string block_common_delim){
  std::string block_raw_text = get_substring_with(block_start);
  int num_lines_in_block = get_num_occurrences_of_substr_in_str(block_common_delim, block_raw_text);
  int num_commands = num_lines_in_block - NUM_NEWLINES_IN_PADDING;
  std::string* commands = new std::string[num_commands];
  std::size_t arr_index = 0;
  //std::cout << block_start << std::endl;
  for(int i = block_raw_text.find(block_common_delim); i!= -1;){
    commands[arr_index] = get_line(block_raw_text, i + 1);
    //std::cout << commands[arr_index] << std::endl;
    i = block_raw_text.find(block_common_delim, i + 1);
    arr_index++;
  }
  //std::cout << std::endl;
  return commands;
}

std::string* Tester::read_includes_in_command_file(){
  // std::string includes_raw_text = get_substring_with("INCLUDE:");
  // int num_lines_in_include = get_num_occurrences_of_substr_in_str("\n", includes_raw_text);
  // int num_lines_of_commands_in_include = num_lines_in_include - NUM_NEWLINES_IN_PADDING;
  // std::string* includes = new std::string[num_lines_of_commands_in_include];
  // std::size_t index_of_includes = 0;
  // for(int i = includes_raw_text.find("\n"); i != -1;){
  //   includes[index_of_includes] = get_line(includes_raw_text, i + 1);
  //   std::cout << includes[index_of_includes] << std::endl;
  //   i = includes_raw_text.find("\n", i + 1);
  //   index_of_includes++;
  // }
  std::string* includes = return_block_as_array_of_lines("INCLUDE:", "\n");
  return includes;
}

std::string* Tester::read_defines_in_command_file(){
  std::string* defines = return_block_as_array_of_lines("DEFINE:", "\n");
  return defines;
}

std::string* Tester::read_methods_in_command_file(){
  std::string* methods = return_block_as_array_of_lines("METHOD:", "\n");
  return methods;
}

std::string* Tester::read_params_in_command_file(){
  std::string* params = return_block_as_array_of_lines("PARAM:", "\n");
  return params;
}

std::string* Tester::read_dos_in_command_file(){
  std::string* dos = return_block_as_array_of_lines("DO:", "\n");
  return dos;
}
