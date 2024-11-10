#include <iostream>
#include <fstream>
#include <vector>
#include "emulator.hpp"

using namespace std;

// ANSI color codes
const char *RESET = "\033[0m";
const char *RED = "\033[31m";
const char *GREEN = "\033[32m";
const char *YELLOW = "\033[33m";
const char *BLUE = "\033[34m";
const char *CYAN = "\033[36m";
const char *MAGENTA = "\033[35m";

Instruction Emulator::decode(uint8_t file_byte)
{
    uint8_t opcode = (file_byte >> 4) & 0xF;
    uint8_t imm;
    uint8_t reg[2];
    uint8_t mask = 0b11;
    uint8_t opmask = ~((opcode >> 1) ^ mask);
    if (opmask)
    {
        imm = file_byte & 0xF;
    }
    else
    {
        reg[1] = file_byte & 3;
        reg[0] = (file_byte >> 2) & 3;
    }
    Instruction instr = Instruction(opcode, imm, reg);
    return instr;
}

void Emulator::run()
{
    ifstream file(this->filename, ios::binary | ios::ate);
    if (!file)
    {
        cerr << RED << "Error opening file: " << this->filename << RESET << endl;
        return;
    }
    streamsize size = file.tellg();
    file.seekg(0, ios::beg);
    vector<char> bytecode(size);
    if (!file.read(bytecode.data(), size))
    {
        cerr << RED << "Error reading file: " << this->filename << RESET << endl;
        return;
    }
    // for (int i = 0; i < size; i++)
    // {
    //     debug_print("%X ", bytecode[i]);
    // }
    pc = 0;
    while (true)
    {
        uint8_t op = bytecode[pc];
        Instruction instr = decode(op);
        execute(instr);
        if (flags & 0b1000)
        {
            break;
        }
        else if (flags & 0b0100)
        {
            debug_print("%sException occurred. Exiting...%s\n", RED, RESET);
            break;
        }
    }
    file.close();
}

uint8_t Emulator::execute(Instruction instr)
{
    uint16_t imm;
    uint8_t q, r;
    switch (instr.info.op)
    {
    case ADD:
        debug_print("%s[%X] ADD R%d, R%d%s\n", CYAN, pc, instr.reg[0], instr.reg[1], RESET);
        // debug_print("R%d -> 0x%X, R%d -> 0x%X\n", instr.reg[0], reg[instr.reg[0]], instr.reg[1], reg[instr.reg[1]]);
        reg[instr.reg[0]] += reg[instr.reg[1]];
        // debug_print("R%d -> 0x%X\n", instr.reg[0], reg[instr.reg[0]]);
        pc++;
        break;
    case SUB:
        debug_print("%s[%X] SUB R%d, R%d%s\n", CYAN, pc, instr.reg[0], instr.reg[1], RESET);
        // debug_print("R%d -> 0x%X, R%d -> 0x%X\n", instr.reg[0], reg[instr.reg[0]], instr.reg[1], reg[instr.reg[1]]);
        reg[instr.reg[0]] -= reg[instr.reg[1]];
        // debug_print("R%d -> 0x%X\n", instr.reg[0], reg[instr.reg[0]]);
        pc++;
        break;
    case MUL:
        debug_print("%s[%X] MUL R%d, R%d%s\n", CYAN, pc, instr.reg[0], instr.reg[1], RESET);
        // debug_print("R%d -> 0x%X, R%d -> 0x%X\n", instr.reg[0], reg[instr.reg[0]], instr.reg[1], reg[instr.reg[1]]);
        imm = (uint16_t)reg[instr.reg[0]] * (uint16_t)reg[instr.reg[1]];
        reg[0] = imm & 0xF;
        reg[1] = imm >> 4;
        // debug_print("R0 -> 0x%X, R1 -> 0x%X\n", reg[0], reg[1]);
        pc++;
        break;
    case DIV:
        debug_print("%s[%X] DIV R%d, R%d%s\n", CYAN, pc, instr.reg[0], instr.reg[1], RESET);
        // debug_print("R%d -> 0x%X, R%d -> 0x%X\n", instr.reg[0], reg[instr.reg[0]], instr.reg[1], reg[instr.reg[1]]);
        q = reg[instr.reg[0]] / reg[instr.reg[1]];
        r = reg[instr.reg[0]] % reg[instr.reg[1]];
        reg[0] = q;
        reg[1] = r;
        // debug_print("R0 -> 0x%X, R1 -> 0x%X\n", reg[0], reg[1]);
        pc++;
        break;
    case NAND:
        debug_print("%s[%X] NAND R%d, R%d%s\n", CYAN, pc, instr.reg[0], instr.reg[1], RESET);
        // debug_print("R%d -> 0x%X, R%d -> 0x%X\n", instr.reg[0], reg[instr.reg[0]], instr.reg[1], reg[instr.reg[1]]);
        reg[instr.reg[0]] = ~(reg[instr.reg[0]] & reg[instr.reg[1]]);
        // debug_print("R%d -> 0x%X\n", instr.reg[0], reg[instr.reg[0]]);
        pc++;
        break;
    case CMP:
        debug_print("%s[%X] CMP R%d, R%d%s\n", MAGENTA, pc, instr.reg[0], instr.reg[1], RESET);
        // debug_print("R%d -> 0x%X, R%d -> 0x%X\n", instr.reg[0], reg[instr.reg[0]], instr.reg[1], reg[instr.reg[1]]);
        if (reg[instr.reg[0]] == reg[instr.reg[1]])
        {
            flags = 0b0001;
        }
        else if (reg[instr.reg[0]] > reg[instr.reg[1]])
        {
            flags = 0b0010;
        }
        pc++;
        // debug_print("Flags -> 0x%X\n", flags);
        break;
    case JMP:
        debug_print("%s[%X] JMP 0x%X%s\n", YELLOW, pc, instr.imm, RESET);
        pc++;
        pc += instr.imm;
        // debug_print("PC -> 0x%X\n", pc);
        break;
    case JE:
        debug_print("%s[%X] JE 0x%X%s\n", YELLOW, pc, instr.imm, RESET);
        pc++;
        if (flags & 0b0001)
        {
            pc += instr.imm;
        }
        // debug_print("PC -> 0x%X\n", pc);
        break;
    case JGT:
        debug_print("%s[%X] JGT 0x%X%s\n", YELLOW, pc, instr.imm, RESET);
        pc++;
        if (flags & 0b0010)
        {
            pc += instr.imm;
        }
        // debug_print("PC -> 0x%X\n", pc);
        break;

    case LD:
        debug_print("%s[%X] LD R%d, R%d%s\n", BLUE, pc, instr.reg[0], instr.reg[1], RESET);
        // debug_print("R%d -> 0x%X, R%d -> 0x%X\n", instr.reg[0], reg[instr.reg[0]], instr.reg[1], reg[instr.reg[1]]);
        reg[instr.reg[0]] = memory[reg[instr.reg[1]]];
        // debug_print("R%d -> 0x%X\n", instr.reg[0], reg[instr.reg[0]]);
        pc++;
        break;

    case ST:
        debug_print("%s[%X] ST R%d, R%d%s\n", BLUE, pc, instr.reg[0], instr.reg[1], RESET);
        // debug_print("R%d -> 0x%X, R%d -> 0x%X\n", instr.reg[0], reg[instr.reg[0]], instr.reg[1], reg[instr.reg[1]]);
        memory[reg[instr.reg[0]]] = reg[instr.reg[1]];
        // debug_print("Memory[0x%X] -> 0x%X\n", reg[instr.reg[0]], memory[reg[instr.reg[0]]]);
        pc++;
        break;
    case MOV:
        debug_print("%s[%X] MOV R%d, R%d%s\n", GREEN, pc, instr.reg[0], instr.reg[1], RESET);
        // debug_print("R%d -> 0x%X, R%d -> 0x%X\n", instr.reg[0], reg[instr.reg[0]], instr.reg[1], reg[instr.reg[1]]);
        reg[instr.reg[0]] = reg[instr.reg[1]];
        // debug_print("R%d -> 0x%X\n", instr.reg[0], reg[instr.reg[0]]);
        pc++;
        break;
    case MOVL:
        debug_print("%s[%X] MOVL $0x%X%s\n", GREEN, pc, instr.imm, RESET);
        reg[0] = instr.imm;
        // debug_print("R0 -> 0x%X\n", reg[instr.reg[0]]);
        pc++;
        break;
    case MOVH:
        debug_print("%s[%X] MOVH $0x%X%s\n", GREEN, pc, instr.imm, RESET);
        reg[0] = instr.imm << 4;
        // debug_print("R0 -> 0x%X\n", reg[instr.reg[0]]);
        pc++;
        break;
    case SYSCALL:
        debug_print("%s[%X] SYSCALL%s\n", RED, pc, RESET);
        switch (reg[0])
        {
        case 0x0:

            for (int i = 0; i < reg[3]; i++)
            {
                cout << memory[reg[2] + i];
            }
            break;
        case 0x1:

            for (int i = 0; i < reg[3]; i++)
            {
                cin >> memory[reg[2] + i];
            }
            break;
        default:
            debug_print("%sInvalid syscall%s\n", RED, RESET);
            flags = 0b0100;
        }

        pc++;
        break;
    case HLT:
        debug_print("%s[%X] HLT%s\n", RED, pc, RESET);
        flags = 0b1000;
        pc++;
        break;
    default:
        debug_print("%sInvalid instruction%s\n", RED, RESET);
        flags = 0b0100;
    }
    return 0;
}
