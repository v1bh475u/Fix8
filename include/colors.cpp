#include "colors.hpp"

const char *RESET = "\033[0m";
const char *RED = "\033[31m";
const char *GREEN = "\033[32m";
const char *YELLOW = "\033[33m";
const char *BLUE = "\033[34m";
const char *CYAN = "\033[36m";
const char *MAGENTA = "\033[35m";

map<Operation, string> instruction_colors = {
    {ADD, GREEN},
    {SUB, GREEN},
    {MUL, GREEN},
    {DIV, GREEN},
    {NAND, CYAN},
    {JMP, YELLOW},
    {JE, YELLOW},
    {JGT, YELLOW},
    {CMP, MAGENTA},
    {LD, BLUE},
    {ST, BLUE},
    {MOV, BLUE},
    {MOVL, BLUE},
    {MOVH, BLUE},
    {SYSCALL, RED},
    {HLT, RED}};