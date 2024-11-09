#pragma once

#include <string>
#include <vector>
#include <map>
#include <optional>
#include "isa.hpp"

using namespace std;

class Assembler
{
private:
    string filename;
    string current_line;
    int line_num, column_num;
    vector<uint8_t> machine_code;

    optional<uint8_t> parse_line(string line);
    optional<Instruction> parse_instruction(vector<string> &tokens);
    optional<uint8_t> parse_register(string reg);
    optional<uint8_t> parse_immediate(string imm);
    void parse_operands_rtype(Instruction &instruction, vector<string> &tokens);
    void parse_operands_itype(Instruction &instruction, vector<string> &tokens);
    void parse_operands_ntype(Instruction &instruction, vector<string> &tokens);
    void handle_error(string message);

public:
    Assembler();
    ~Assembler();
    vector<uint8_t> assemble(string filename);
};
