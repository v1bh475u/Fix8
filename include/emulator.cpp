#include <iostream>
#include <fstream>
#include <vector>
#include "emulator.hpp"
#include "colors.hpp"
#include <limits>

using namespace std;

Instruction Emulator::decode(uint8_t file_byte)
{
    uint8_t opcode = (file_byte >> 4) & 0xF;
    uint8_t imm;
    uint8_t reg[2];
    uint8_t mask = 0b011;
    uint8_t opmask = ((opcode >> 1) ^ mask);
    if (opmask == 0)
    {
        imm = file_byte & 0xF;
    }
    else
    {
        reg[1] = file_byte & 0b11;
        reg[0] = (file_byte >> 2) & 0b11;
    }
    Instruction instr = Instruction(opcode, imm, reg);
    return instr;
}

void Emulator::run_program(string filename)
{
    this->filename = filename;

    ifstream file(filename, ios::binary);
    if (!file)
    {
        cerr << RED << "Error opening file: " << this->filename << RESET << endl;
        return;
    }
    this->bytecode = vector<int8_t>((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    this->pc = 0;
    this->flags = 0;
    while (true)
    {
        uint8_t byte = this->bytecode[this->pc];
        Instruction instr = decode(byte);
        this->print_regs({0, 1, 2, 3});
        this->print_instr(instr);
        execute(instr);
        if (this->flags & 0b1000)
        {
            break;
        }
        else if (this->flags & 0b0100)
        {
            debug_print("%sException occurred. Exiting...%s\n", RED, RESET);
            break;
        }
        if (this->pc >= this->bytecode.size())
        {
            debug_print("%sEnd of program reached.%s\n", GREEN, RESET);
            break;
        }
    }
    file.close();
}

uint8_t Emulator::execute(Instruction instr)
{
    uint16_t imm;
    uint8_t q, r;
    int16_t offset;
    offset = (reg[instr.reg[0]] << 4) | reg[instr.reg[1]];
    switch (instr.info.op)
    {
    case ADD:
        reg[instr.reg[0]] += reg[instr.reg[1]];
        this->pc++;
        break;
    case SUB:
        reg[instr.reg[0]] -= reg[instr.reg[1]];
        this->pc++;
        break;
    case MUL:
        imm = (uint16_t)reg[instr.reg[0]] * (uint16_t)reg[instr.reg[1]];
        reg[0] = imm & 0xF;
        reg[1] = imm >> 4;
        this->pc++;
        break;
    case DIV:
        q = reg[instr.reg[0]] / reg[instr.reg[1]];
        r = reg[instr.reg[0]] % reg[instr.reg[1]];
        reg[0] = q;
        reg[1] = r;
        this->pc++;
        break;
    case NAND:
        reg[instr.reg[0]] = ~(reg[instr.reg[0]] & reg[instr.reg[1]]);
        this->pc++;
        break;
    case CMP:
        if (reg[instr.reg[0]] == reg[instr.reg[1]])
        {
            this->flags = 0b0001;
        }
        else if (reg[instr.reg[0]] > reg[instr.reg[1]])
        {
            this->flags = 0b0010;
        }
        this->pc++;
        this->print_flags();
        break;
    case JMP:
        this->pc++;
        this->pc += offset;
        this->print_pc();
        break;
    case JE:
        this->pc++;
        if (this->flags & 0b0001)
        {
            this->pc += offset;
        }
        this->print_pc();
        break;

    case JGT:
        this->pc++;
        if (this->flags & 0b0010)
        {
            this->pc += offset;
        }
        this->print_pc();
        break;

    case LD:
        reg[instr.reg[0]] = memory[reg[instr.reg[1]]];
        this->pc++;
        break;

    case ST:
        memory[reg[instr.reg[0]]] = reg[instr.reg[1]];
        this->print_memory({reg[instr.reg[0]]});
        this->pc++;
        break;
    case MOV:
        reg[instr.reg[0]] = reg[instr.reg[1]];
        this->pc++;
        break;
    case MOVL:
        reg[0] = instr.imm;
        this->pc++;
        break;
    case MOVH:
        reg[0] = instr.imm << 4;
        this->pc++;
        break;
    case SYSCALL:
        switch (reg[0])
        {
        case 0x0:
            for (int i = 0; i < reg[3]; i++)
                cout << static_cast<char>(memory[reg[2] + i]);
            cout << endl;
            break;
        case 0x1:
            cin.clear();
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
            for (int i = 0; i < reg[3]; i++)
                memory[reg[2] + i] = cin.get();
            break;
        default:
            debug_print("%sInvalid syscall!%s\n", RED, RESET);
            this->flags = 0b0100;
        }
        this->pc++;
        break;
    case HLT:
        this->flags = 0b1000;
        this->pc++;
        break;
    default:
        debug_print("%sInvalid instruction!%s\n", RED, RESET);
        this->flags = 0b0100;
    }
    return 0;
}
