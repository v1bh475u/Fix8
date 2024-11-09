#include <iostream>
#include <fstream>
#include <vector>
#include "emulator.hpp"

Instruction Emulator::decode(uint8_t file_byte)
{
    uint8_t opcode = (file_byte >> 4) & 0xF;
    uint8_t imm;
    uint8_t reg[2];
    if (opcode == 0b1110)
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
    ifstream file(this->filename, ios::binary);
    if (!file)
    {
        cerr << "Error opening file: " << this->filename << endl;
        return;
    }
    vector<char> bytecode((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    for (pc = 0; pc < bytecode.size();)
    {
        uint8_t byte = bytecode[pc];
        // debug_print("Byte=%x\n", byte);
        Instruction instr = decode(byte);
        execute(instr);
        if (flags & 0b1000)
        {
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
        debug_print("[%X] ADD R%d, R%d\n", pc, instr.reg[0], instr.reg[1]);
        debug_print("R%d=%x, R%d=%x\n", instr.reg[0], reg[instr.reg[0]], instr.reg[1], reg[instr.reg[1]]);
        reg[instr.reg[0]] += reg[instr.reg[1]];
        debug_print("R%d=%x\n", instr.reg[0], reg[instr.reg[0]]);
        pc++;
        break;
    case SUB:
        debug_print("[%X] SUB R%d, R%d\n", pc, instr.reg[0], instr.reg[1]);
        debug_print("R%d=%x, R%d=%x\n", instr.reg[0], reg[instr.reg[0]], instr.reg[1], reg[instr.reg[1]]);
        reg[instr.reg[0]] -= reg[instr.reg[1]];
        debug_print("R%d=%x\n", instr.reg[0], reg[instr.reg[0]]);
        pc++;
        break;
    case MUL:
        debug_print("[%X] MUL R%d, R%d\n", pc, instr.reg[0], instr.reg[1]);
        debug_print("R%d=%x, R%d=%x\n", instr.reg[0], reg[instr.reg[0]], instr.reg[1], reg[instr.reg[1]]);
        imm = (uint16_t)reg[instr.reg[0]] * (uint16_t)reg[instr.reg[1]];
        reg[0] = imm & 0xF;
        reg[1] = imm >> 4;
        debug_print("R0=%x, R1=%x\n", reg[0], reg[1]);
        pc++;
        break;
    case DIV:
        debug_print("[%X] DIV R%d, R%d\n", pc, instr.reg[0], instr.reg[1]);
        debug_print("R%d=%x, R%d=%x\n", instr.reg[0], reg[instr.reg[0]], instr.reg[1], reg[instr.reg[1]]);
        q = reg[instr.reg[0]] / reg[instr.reg[1]];
        r = reg[instr.reg[0]] % reg[instr.reg[1]];
        reg[0] = q;
        reg[1] = r;
        debug_print("R0=%x, R1=%x\n", reg[0], reg[1]);
        pc++;
        break;
    case SHR:
        pc++;
        debug_print("[%X] SHR R%d, R%d\n", pc, instr.reg[0], instr.reg[1]);
        debug_print("R%d=%x, R%d=%x\n", instr.reg[0], reg[instr.reg[0]], instr.reg[1], reg[instr.reg[1]]);
        reg[instr.reg[0]] >>= reg[instr.reg[1]];
        debug_print("R%d=%x\n", instr.reg[0], reg[instr.reg[0]]);
        break;
    case SHL:
        debug_print("[%X] SHL R%d, R%d\n", pc, instr.reg[0], instr.reg[1]);
        debug_print("R%d=%x, R%d=%x\n", instr.reg[0], reg[instr.reg[0]], instr.reg[1], reg[instr.reg[1]]);
        reg[instr.reg[0]] <<= reg[instr.reg[1]];
        debug_print("R%d=%x\n", instr.reg[0], reg[instr.reg[0]]);
        pc++;
        break;
    case NAND:
        debug_print("[%X] NAND R%d, R%d\n", pc, instr.reg[0], instr.reg[1]);
        debug_print("R%d=%x, R%d=%x\n", instr.reg[0], reg[instr.reg[0]], instr.reg[1], reg[instr.reg[1]]);
        reg[instr.reg[0]] = ~(reg[instr.reg[0]] & reg[instr.reg[1]]);
        debug_print("R%d=%x\n", instr.reg[0], reg[instr.reg[0]]);
        pc++;
        break;
    case CMP:
        debug_print("[%X] CMP R%d, R%d\n", pc, instr.reg[0], instr.reg[1]);
        debug_print("R%d=%x, R%d=%x\n", instr.reg[0], reg[instr.reg[0]], instr.reg[1], reg[instr.reg[1]]);
        if (reg[instr.reg[0]] == reg[instr.reg[1]])
        {
            flags = 0b0001;
        }
        else if (reg[instr.reg[0]] > reg[instr.reg[1]])
        {
            flags = 0b0010;
        }
        pc++;
        debug_print("Flags=%x\n", flags);
        break;
    case JMP:
        debug_print("[%X] JMP %x\n", pc, instr.imm);
        pc = instr.imm;
        debug_print("PC=%x\n", pc);
        break;
    case JE:
        debug_print("[%X] JE %x\n", pc, instr.imm);
        if (flags & 0b0001)
        {
            pc = instr.imm;
        }
        else
        {
            pc++;
        }
        debug_print("PC=%x\n", pc);
        break;
    case JGT:
        debug_print("[%X] JGT %x\n", pc, instr.imm);
        if (flags & 0b0010)
        {
            pc = instr.imm;
        }
        else
        {
            pc++;
        }
        debug_print("PC=%x\n", pc);
        break;
    case LD:
        debug_print("[%X] LD R%d, R%d\n", pc, instr.reg[0], instr.reg[1]);
        debug_print("R%d=%x, R%d=%x\n", instr.reg[0], reg[instr.reg[0]], instr.reg[1], reg[instr.reg[1]]);
        debug_print("R0=%x\n", reg[0]);
        reg[instr.reg[0]] = memory[(reg[instr.reg[1]] << 8) + reg[instr.reg[1]]];
        debug_print("R0=%x\n", reg[instr.reg[0]]);
        pc++;
        break;
    case ST:
        debug_print("[%X] ST R%d, R%d\n", pc, instr.reg[0], instr.reg[1]);
        debug_print("R%d=%x, R%d=%x\n", instr.reg[0], reg[instr.reg[0]], instr.reg[1], reg[instr.reg[1]]);
        debug_print("R0=%x\n", reg[0]);
        memory[(reg[instr.reg[1]] << 8) + reg[instr.reg[1]]] = reg[instr.reg[0]];
        debug_print("Memory[%X]=%x\n", (reg[instr.reg[1]] << 8) + reg[instr.reg[1]], memory[(reg[instr.reg[1]] << 8) + reg[instr.reg[1]]]);
        pc++;
        break;
    case MOV:
        debug_print("[%X] MOV R%d, R%d\n", pc, instr.reg[0], instr.reg[1]);
        debug_print("R%d=%x, R%d=%x\n", instr.reg[0], reg[instr.reg[0]], instr.reg[1], reg[instr.reg[1]]);
        reg[instr.reg[0]] = reg[instr.reg[1]];
        debug_print("R%d=%x\n", instr.reg[0], reg[instr.reg[0]]);
        pc++;
        break;
    case MOVI:
        debug_print("[%X] MOVI $%x\n", pc, instr.imm);
        reg[0] = instr.imm;
        debug_print("R0=%x\n", reg[instr.reg[0]]);
        pc++;
        break;
    case HLT:
        debug_print("[%X] HLT\n", pc);
        flags = 0b1000;
        pc++;
        break;
    }
    return 0;
}
