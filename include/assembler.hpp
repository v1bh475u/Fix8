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
    vector<byte> machine_code;

    optional<byte> parse_line(string line);
    optional<Instruction> parse_instruction(vector<string> &tokens);
    optional<byte> parse_register(string reg);
    optional<byte> parse_immediate(string imm);
    void parse_operands_rtype(Instruction &instruction, vector<string> &tokens);
    void parse_operands_itype(Instruction &instruction, vector<string> &tokens);
    void parse_operands_ntype(Instruction &instruction, vector<string> &tokens);
    void handle_error(string message);

public:
    Assembler();
    ~Assembler();
    vector<byte> assemble(string filename);
};
