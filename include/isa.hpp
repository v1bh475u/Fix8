#ifndef ISA_HPP
#define ISA_HPP
#include <string>
#include <map>

using namespace std;

enum Register
{
    R0 = 0b00,
    R1 = 0b01,
    R2 = 0b10,
    R3 = 0b11
};

map<string, Register> registers = {
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
    SHR = 0b0101,
    SHL = 0b0110,
    NAND = 0b0111,
    CMP = 0b0100,
    JMP = 0b1000,
    JE = 0b1001,
    JGT = 0b1010,
    LD = 0b1011,
    ST = 0b1100,
    MOV = 0b1101,
    MOVI = 0b1110,
    HLT = 0b1111,
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

map<string, OpInfo> instruction_set = {
    {"ADD", {ADD, R}},
    {"SUB", {SUB, R}},
    {"MUL", {MUL, R}},
    {"DIV", {DIV, R}},
    {"SHR", {SHR, R}},
    {"SHL", {SHL, R}},
    {"NAND", {NAND, R}},
    {"CMP", {CMP, R}},
    {"JMP", {JMP, I}},
    {"JE", {JE, I}},
    {"JGT", {JGT, I}},
    {"LD", {LD, R}},
    {"ST", {ST, R}},
    {"MOV", {MOV, R}},
    {"MOVI", {MOVI, I}},
    {"HLT", {HLT, N}}};

class Instruction
{
public:
    OpInfo info;
    byte imm;
    byte reg[2];

    Instruction(string op)
    {
        this->info = instruction_set[op];
    }

    byte value()
    {
        byte opcode = byte(info.op);
        InstructionFormat format = info.format;

        byte code = byte(opcode << 4);
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
};

#endif // ISA_HPP