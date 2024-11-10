#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <optional>
#include <bitset>
#include <cstdint>
#include "assembler.hpp"
#include "colors.hpp"

using namespace std;

Assembler::Assembler()
{
    line_num = 0;
    column_num = 0;
}

Assembler::~Assembler()
{
}

vector<uint8_t> Assembler::assemble(string filename)
{
    this->filename = filename;
    ifstream file(filename);
    if (!file.is_open())
        handle_error("Could not open file: " + filename);

    vector<string> lines;
    string line;
    while (getline(file, line))
    {
        lines.push_back(line);
    }

    for (line_num = 0; line_num < (int)lines.size(); line_num++)
    {
        this->current_line = lines[line_num];
        this->column_num = 0;

        optional<uint8_t> instruction = parse_line(current_line);
        if (instruction.has_value())
            machine_code.push_back(instruction.value());
    }

    return machine_code;
}

optional<uint8_t> Assembler::parse_line(string line)
{
    vector<string> tokens;
    stringstream ss(line);
    string token;
    uint8_t code = uint8_t(0x00);

    while (ss >> token)
    {
        // Handling comments
        if (token[0] == ';')
            break;
        else if (token[0] == '\t')
            continue;

        tokens.push_back(token);
    }

    if (tokens.empty())
        return nullopt;
    else
    {
        auto instruction = parse_instruction(tokens);

        if (instruction.has_value())
            code = instruction.value().get_byte_code();
        else
        {
            return nullopt;
        }
    }

    return code;
}

optional<Instruction> Assembler::parse_instruction(vector<string> &tokens)
{
    string op = tokens.front();
    tokens.erase(tokens.begin());

    if (instruction_set.find(op) == instruction_set.end())
    {
        handle_error("Invalid instruction: " + op);
        return nullopt;
    }

    Instruction instruction(op);
    this->column_num += op.size() + 1;

    switch (instruction.info.format)
    {
    case R:
        parse_operands_rtype(instruction, tokens);
        break;
    case I:
        parse_operands_itype(instruction, tokens);
        break;
    case N:
        parse_operands_ntype(instruction, tokens);
        break;
    default:
        handle_error("Invalid instruction format");
        return nullopt;
    }

    return instruction;
}

void Assembler::parse_operands_rtype(Instruction &instruction, vector<string> &tokens)
{
    if (tokens.size() != 2)
    {
        handle_error("Invalid number of operands");
        return;
    }

    optional<uint8_t> rd = parse_register(tokens[0]);
    this->column_num += tokens[0].size() + 1;
    optional<uint8_t> rs = parse_register(tokens[1]);
    this->column_num += tokens[1].size() + 1;

    if (rd.has_value() && rs.has_value())
    {
        instruction.reg[0] = rd.value();
        instruction.reg[1] = rs.value();
    }
}

void Assembler::parse_operands_itype(Instruction &instruction, vector<string> &tokens)
{
    if (tokens.size() != 1)
    {
        handle_error("Invalid number of operands.");
        return;
    }

    optional<uint8_t> imm = parse_immediate(tokens[0]);
    if (imm.has_value())
    {
        instruction.imm = imm.value();
    }

    this->column_num += tokens[0].size() + 1;
}

void Assembler::parse_operands_ntype(Instruction &instruction, vector<string> &tokens)
{
    if (tokens.size() != 0)
    {
        handle_error("Invalid number of operands!");
        return;
    }
}

optional<uint8_t> Assembler::parse_register(string reg)
{
    if (reg.back() == ',')
        reg.pop_back();

    if (registers.find(reg) != registers.end())
        return uint8_t(registers[reg]);

    else
    {
        this->handle_error("Invalid register: " + reg);
    }

    return nullopt;
}

optional<uint8_t> Assembler::parse_immediate(string imm)
{
    if (imm[0] == '$')
        imm = imm.substr(1);

    if (imm.back() == ',')
        imm.pop_back();

    try
    {
        return uint8_t(stoi(imm, nullptr, 16));
    }
    catch (...)
    {
        handle_error("Invalid immediate: " + imm);
        return nullopt;
    }
}

void Assembler::handle_error(string message)
{
    cout << RED << this->filename << " : Error at " << line_num << ":" << column_num << RESET << endl;
    cout << RED << current_line << RESET << endl;
    cout << RED << message << RESET << endl;
    throw runtime_error("Assembler error!");
}