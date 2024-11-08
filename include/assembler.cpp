#include "include/instruction.hpp"
#include "include/assembler.hpp"
using namespace std;
void Assembler::assemble()
{
    read_file();
    parse_lines();
    write_output();
    print_errors();
}

void Assembler::read_file()
{
    ifstream file(input_file);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << input_file << endl;
        exit(1);
    }

    string line;
    while (getline(file, line))
    {
        lines.push_back(line);
    }

    file.close();
}

void Assembler::parse_lines()
{
    for (size_t i = 0; i < lines.size(); ++i)
    {
        handle_line(lines[i], i + 1);
    }
}

void Assembler::handle_line(const string &line, int line_number)
{
    string trimmed_line = regex_replace(line, regex("^\\s+|\\s+$"), "");
    if (trimmed_line.empty() || trimmed_line[0] == '#')
    {
        return;
    }

    if (trimmed_line.find("/*") != string::npos)
    {
        while (trimmed_line.find("*/") == string::npos && line_number < lines.size())
        {
            ++line_number;
        }
        return;
    }

    if (trimmed_line.back() == ':')
    {
        handle_label(trimmed_line.substr(0, trimmed_line.size() - 1), line_number);
    }
    else
    {
        stringstream ss(trimmed_line);
        vector<string> tokens;
        string token;
        while (ss >> token)
        {
            tokens.push_back(token);
        }
        handle_instruction(tokens, line_number);
    }
}

void Assembler::handle_instruction(const vector<string> &tokens, int line_number)
{
    if (tokens.empty())
    {
        return;
    }

    string instruction_name = tokens[0];
    if (instructions.find(instruction_name) == instructions.end())
    {
        handle_error("Unknown instruction: " + instruction_name, line_number, 0);
        return;
    }

    Instruction instruction = instructions[instruction_name];
    stringstream binary_instruction;
    binary_instruction << bitset<4>(instruction.opcode);

    try
    {
        switch (instruction.format)
        {
        case R:
            if (tokens.size() != 3)
            {
                handle_error("Invalid number of operands for R format", line_number, 0);
                return;
            }
            if (registers.find(tokens[1]) == registers.end() || registers.find(tokens[2]) == registers.end())
            {
                handle_error("Invalid register ", line_number, 0);
                return;
            }
            binary_instruction << bitset<2>(registers[tokens[1]]) << bitset<2>(registers[tokens[2]]);
            break;

        case I:
            if (tokens.size() != 3)
            {
                handle_error("Invalid number of operands for I format", line_number, 0);
                return;
            }
            if (registers.find(tokens[1]) == registers.end())
            {
                handle_error("Invalid register", line_number, 0);
                return;
            }
            binary_instruction << bitset<2>(registers[tokens[1]]) << bitset<4>(parse_immediate(tokens[2]));
            break;
        case O:
            if (tokens.size() != 1)
            {
                handle_error("Invalid number of operands for O format", line_number, 0);
                return;
            }
            binary_instruction << bitset<6>(0);
            break;

        default:
            handle_error("Unknown instruction format", line_number, 0);
            return;
        }
    }
    catch (const exception &e)
    {
        handle_error("Error processing instruction: " + string(e.what()), line_number, 0);
        return;
    }

    lines[line_number - 1] = binary_instruction.str();
}

void Assembler::handle_label(const string &label, int line_number)
{
    string lower_label = label;
    transform(lower_label.begin(), lower_label.end(), lower_label.begin(), ::tolower);
    if (labels.find(lower_label) != labels.end())
    {
        handle_error("Duplicate label: " + label, line_number, 0);
        return;
    }

    labels[lower_label] = line_number;
}

void Assembler::handle_error(const string &message, int line_number, int column)
{
    stringstream ss;
    ss << "\033[1;31m" << "[ERROR] " << "\033[0m" << line_number << "," << column << ": " << message << " | Line: " << lines[line_number - 1];
    errors.push_back(ss.str());
}

int Assembler::parse_immediate(const string &immediate)
{
    if (immediate.find("0x") == 0)
    {
        return stoi(immediate, nullptr, 16);
    }
    else if (immediate.find("0b") == 0)
    {
        return stoi(immediate.substr(2), nullptr, 2);
    }
    else
    {
        return stoi(immediate);
    }
}

void Assembler::write_output()
{
    ofstream file(output_file);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << output_file << endl;
        exit(1);
    }

    for (size_t i = 0; i < lines.size(); ++i)
    {
        file << i + 1 << ": " << lines[i] << endl;
    }

    file.close();
}

void Assembler::print_errors()
{
    for (const string &error : errors)
    {
        cerr << error << endl;
    }
}
