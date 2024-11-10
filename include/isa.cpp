#include "isa.hpp"

map<string, Register> registers = {
    {"R0", R0},
    {"R1", R1},
    {"R2", R2},
    {"R3", R3}};

map<string, OpInfo> instruction_set = {
    {"ADD", {ADD, R}},
    {"SUB", {SUB, R}},
    {"MUL", {MUL, R}},
    {"DIV", {DIV, R}},
    {"NAND", {NAND, R}},
    {"JMP", {JMP, R}},
    {"JE", {JE, R}},
    {"JGT", {JGT, R}},
    {"CMP", {CMP, R}},
    {"LD", {LD, R}},
    {"ST", {ST, R}},
    {"MOV", {MOV, R}},
    {"MOVL", {MOVL, I}},
    {"MOVH", {MOVH, I}},
    {"SYSCALL", {SYSCALL, N}},
    {"HLT", {HLT, N}}};

Instruction::Instruction(string op)
{
    this->info = instruction_set[op];
}

Instruction::Instruction(uint8_t opcode, uint8_t imm, uint8_t *reg)

{
    this->info.op = static_cast<Operation>(opcode);

    for (auto &i : instruction_set)
    {
        if (i.second.op == this->info.op)
        {
            this->info.format = i.second.format;
            break;
        }
    }

    this->imm = imm;
    this->reg[0] = reg[0];
    this->reg[1] = reg[1];
}

uint8_t Instruction::get_byte_code()
{
    uint8_t opcode = uint8_t(this->info.op);
    InstructionFormat format = this->info.format;

    uint8_t code = uint8_t(opcode << 4);
    if (format == N)
    {
        return code;
    }
    else if (format == I)
    {
        code |= imm;
        return code;
    }
    else
    {
        code |= reg[0] << 2;
        code |= reg[1] << 0;
        return code;
    }
}

string Instruction::get_string()
{
    string op = "";
    for (auto &i : instruction_set)
    {
        if (i.second.op == this->info.op)
        {
            op = i.first;
            break;
        }
    }

    string str = op + " ";
    if (this->info.format == I)
        str += to_string(imm);
    else if (this->info.format == R)
        str += "R" + to_string(reg[0]) + ", R" + to_string(reg[1]);

    return str;
}