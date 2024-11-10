#ifndef EMULATOR_HPP
#include <iostream>
#include <cstdint>
#include "isa.hpp"
using namespace std;

#define debug_print(...) printf(__VA_ARGS__)
#define src_reg_print() debug_print("R%d -> 0x%X, R%d -> 0x%X\n", instr.reg[0], reg[instr.reg[0]], instr.reg[1], reg[instr.reg[1]]);
class Emulator
{
public:
    Emulator(const string filename)
    {
        this->filename = filename;
        memory = new int8_t[1 << 8];
    }
    Emulator() : Emulator("")
    {
        memory = new int8_t[1 << 8];
    }
    ~Emulator()
    {
        delete[] memory;
    }
    void load_program(const string filename)
    {
        this->filename = filename;
    }
    void run();
    Instruction decode(uint8_t file_byte);
    uint8_t execute(Instruction instruction);

private:
    string filename;
    int8_t *memory;
    int8_t reg[4];
    uint8_t flags;
    uint8_t pc;
};
#endif