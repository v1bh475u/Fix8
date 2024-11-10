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

extern map<string, Register> registers;

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

extern map<string, OpInfo> instruction_set;

class Instruction
{
public:
    OpInfo info;
    uint8_t imm;
    uint8_t reg[2];
    Instruction(string op);
    Instruction(uint8_t opcode, uint8_t imm, uint8_t *reg);
    uint8_t get_byte_code();
    string get_string();
};
