#ifndef EMULATOR_HPP
#include <iostream>
#include <cstdint>

#include "colors.hpp"
#include "isa.hpp"

using namespace std;

#define debug_print(...) printf(__VA_ARGS__)
class Emulator
{
public:
    Emulator()
    {
        memory = new int8_t[1 << 8];
    }
    ~Emulator()
    {
        delete[] memory;
    }
    void print_regs(initializer_list<int> reg_indices)
    {
        debug_print("%s| ", CYAN);

        for (int idx : reg_indices)
            debug_print("R%d -> %d | ", idx, reg[idx]);

        debug_print("\n%s", RESET);
    }
    void print_instr(Instruction instruction)
    {
        cout << YELLOW << "[" << to_string(this->pc) << "] " << instruction.get_string() << RESET << endl;
    }
    void print_pc()
    {
        debug_print("PC -> %d\n", pc);
    }
    void print_flags()
    {
        debug_print("Flags -> %d\n", flags);
    }
    void print_memory(int mem_addr)
    {
        debug_print("Memory[0x%X] -> 0x%X\n", mem_addr, memory[mem_addr]);
    }

    void run_program(string filename);
    Instruction decode(uint8_t file_byte);
    uint8_t execute(Instruction instruction);

private:
    string filename;
    vector<int8_t> bytecode;
    int8_t *memory;
    int8_t reg[4]{};
    uint8_t flags;
    uint8_t pc;
};
#endif