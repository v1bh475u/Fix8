#include <string>
#include <vector>
#include <map>

enum InstructionFormat
{
    I, // instruction format: opcode, immediate
    R, // instruction format: opcode, reg, reg
    O  // instruction format: opcode
};

struct Instruction
{
    std::string name;
    int opcode;
    InstructionFormat format;
};

std::map<std::string, int>
    registers = {
        {"R0", 0b00},
        {"R1", 0b01},
        {"R2", 0b10},
        {"R3", 0b11},
};

std::map<std::string, Instruction> instructions = {
    // Arithmetic instructions
    {
        "ADD",
        {
            "ADD",
            0b0000,
            R,
        },
    },
    {
        "SUB",
        {
            "SUB",
            0b0001,
            R,
        },
    },
    {
        "MUL",
        {
            "MUL",
            0b0010,
            R,
        },
    },
    {
        "DIV",
        {
            "DIV",
            0b0011,
            R,
        },
    },

    // Data flow instructions
    {
        "MOV",
        {
            "MOV",
            0b1101,
            R,
        },
    },
    {
        "MOVI",
        {
            "MOVI",
            0b1110,
            I,
        },
    },
    {
        "LD",
        {
            "LD",
            0b1011,
            R,
        },
    },
    {
        "ST",
        {
            "ST",
            0b1100,
            R,
        },
    },

    // Bitwise instructions
    {
        "SHR",
        {
            "SHR",
            0b0101,
            R,
        },
    },
    {
        "SHL",
        {
            "SHL",
            0b0110,
            R,
        },
    },
    {
        "NAND",
        {
            "NAND",
            0b0111,
            R,
        },
    },

    // Control flow instructions
    {
        "JUMP",
        {
            "JUMP",
            0b1000,
            R,
        },
    },
    {
        "JE",
        {
            "JE",
            0b1001,
            R,
        },
    },
    {
        "JGT",
        {
            "JGT",
            0b1010,
            R,
        },
    },
    {
        "CMP",
        {
            "CMP",
            0b0100,
            R,
        },
    },
    {
        "HALT",
        {
            "HALT",
            0b1111,
            O,
        },
    },
};
