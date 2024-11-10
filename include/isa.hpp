#pragma once

#include <string>
#include <map>
#include <cstdint>
using namespace std;

enum Register
{
    R0 = 0b00,
    R1 = 0b01,
    R2 = 0b10,
    R3 = 0b11
};

inline map<string, Register> registers = {
    {"R0", R0},
    {"R1", R1},
    {"R2", R2},
    {"R3", R3}};

enum Operation
{
    ADD = 0b0000,
    SUB = 0b0001,
    MUL = 0b0010,
    DIV = 0b0011,
    NAND = 0b0100,
    JMP = 0b1000,
    JE = 0b1001,
    JGT = 0b1010,
    CMP = 0b1011,
    LD = 0b1100,
    ST = 0b1101,
    MOV = 0b0101,
    MOVL = 0b0110,
    MOVH = 0b0111,
    HLT = 0b1111,
    SYSCALL = 0b1110
};

enum InstructionFormat
{
    I, // instruction format: inst, immediate
    R, // instruction format: inst, reg, reg
    N  // instruction format: inst
};

struct OpInfo
{
    Operation op;
    InstructionFormat format;
};

inline map<string, OpInfo> instruction_set = {
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

class Instruction
{
public:
    OpInfo info;
    uint8_t imm;
    uint8_t reg[2];

    Instruction(string op)
    {
        this->info = instruction_set[op];
    }
    Instruction(uint8_t opcode, uint8_t imm, uint8_t *reg)

    {
        this->info.op = static_cast<Operation>(opcode);

        for (auto &i : instruction_set)
        {
            if (i.second.op == info.op)
            {
                this->info.format = i.second.format;
                break;
            }
        }

        this->imm = imm;
        this->reg[0] = reg[0];
        this->reg[1] = reg[1];
    }
    inline uint8_t get_byte_code()
    {
        uint8_t opcode = uint8_t(info.op);
        InstructionFormat format = info.format;

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

    inline string get_string()
    {
        string op = "";
        for (auto &i : instruction_set)
        {
            if (i.second.op == info.op)
            {
                op = i.first;
                break;
            }
        }

        string str = op + " ";
        if (info.format == I)
        {
            str += to_string(imm);
        }
        else if (info.format == R)
        {
            str += "R" + to_string(reg[0]) + ", R" + to_string(reg[1]);
        }

        return str;
    }
};
