
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <regex>
class Assembler
{
public:
    void assemble();
    Assembler(const std::string &input_file, const std::string &output_file)
        : input_file(input_file), output_file(output_file) {}

    void read_file();
    void parse_lines();
    void handle_line(const std::string &line, int line_number);
    void handle_instruction(const std::vector<std::string> &tokens, int line_number);

private:
    std::string input_file;
    std::string output_file;
    std::vector<std::string> lines;
    std::map<std::string, int> labels;
    std::vector<std::string> errors;

    void handle_label(const std::string &label, int line_number);
    void handle_error(const std::string &message, int line_number, int column);
    int parse_immediate(const std::string &immediate);
    void write_output();
    void print_errors();
};
