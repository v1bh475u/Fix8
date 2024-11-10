#pragma once
using namespace std;
#include <string>
#include <map>
#include "isa.hpp"

// ANSI color codes
extern const char *RESET, *RED, *GREEN, *YELLOW, *BLUE, *CYAN, *MAGENTA;
extern map<Operation, string> instruction_colors;