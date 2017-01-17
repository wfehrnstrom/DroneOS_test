#include <fstream>

class Test{
  public:
    Test(std::string *includes, std::string *params, std::string *lines_code);
    ~Test();
    void run();
    void prepare();
    void write_test_main();
    void generate_file();
    void add_imports();
    void add_padding();
    void add_code_body();
    void insert_at(std::size_t line_num, std::string statement);
    void insert_at(std::size_t line_num, std::string *statements);
    bool match_params_with_code_lines(); //Return true if matching was successful, false otherwise
  private:
    bool match_param_and_function_param_type();
    void replace_all_instances(std::string s1, std::string s2);
};
