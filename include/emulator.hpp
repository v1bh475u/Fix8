#ifndef EMULATOR_HPP
#include <iostream>
#include <cstdint>
#include "isa.hpp"
using namespace std;

#define debug_print(...) printf(__VA_ARGS__)

class Emulator
{
public:
    Emulator(const string filename)
    {
        this->filename = filename;
        memory = new uint8_t[1 << 16];
    }
    Emulator() : Emulator("")
    {
        memory = new uint8_t[1 << 16];
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
    uint8_t *memory;
    uint8_t reg[4];
    uint8_t flags;
    uint8_t pc;
};
#endif