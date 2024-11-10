#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

#include "../include/assembler.hpp"
#include "../include/emulator.hpp"
#include "../include/colors.hpp"

using namespace std;
namespace fs = std::filesystem;

int assemble_file(string file_name, string output_file_name)
{
    Assembler assembler;
    vector<uint8_t> machine_code;
    try
    {
        machine_code = assembler.assemble(file_name);
    }
    catch (const exception &e)
    {
        cerr << RED << "Assembly Error: " << e.what() << RESET << endl;
        return 1;
    }

    ofstream output_file(output_file_name, ios::binary);

    if (!output_file)
    {
        cerr << RED << "Error: Could not open the file for writing." << RESET << endl;
        return 1;
    }

    output_file.write(reinterpret_cast<const char *>(machine_code.data()), machine_code.size());
    output_file.close();
    cout << GREEN << "Assembly successful! Output file: " << output_file_name << RESET << endl;
    return 0;
}

int emulate_file(string file_name)
{
    Emulator emulator;
    try
    {
        emulator.load_program(file_name);
        emulator.run();
    }
    catch (const exception &e)
    {
        cerr << RED << "Emulation Error: " << e.what() << RESET << endl;
        return 1;
    }
    return 0;
}

int main()
{
    int choice;
    while (true)
    {
        cout << CYAN << "\n========== Menu ==========\n\n"
             << RESET;
        cout << YELLOW << "1. Assemble a program" << RESET << endl;
        cout << YELLOW << "2. Emulate a program" << RESET << endl;
        cout << YELLOW << "3. Exit\n"
             << RESET << endl;
        cout << BLUE << "Enter your choice: " << RESET;
        cin >> choice;

        if (choice == 1)
        {
            string asm_directory = "../asm";
            vector<string> asm_files;

            for (const auto &entry : fs::directory_iterator(asm_directory))
            {
                if (entry.is_regular_file() && entry.path().extension() == ".asm")
                {
                    asm_files.push_back(entry.path().string());
                }
            }

            if (asm_files.empty())
            {
                cout << RED << "No assembly files found in the directory." << RESET << endl;
                continue;
            }

            cout << GREEN << "Select a file to assemble:\n"
                 << RESET << endl;
            for (size_t i = 0; i < asm_files.size(); ++i)
            {
                cout << GREEN << i + 1 << ". " << asm_files[i] << RESET << endl;
            }

            int file_choice;
            cout << endl;
            cout << BLUE << "Enter your choice: " << RESET;
            cin >> file_choice;

            if (file_choice < 1 || file_choice > static_cast<int>(asm_files.size()))
            {
                cout << RED << "Invalid choice." << RESET << endl;
                continue;
            }

            string selected_file = asm_files[file_choice - 1];
            string base_name = fs::path(selected_file).stem().string();
            string output_file_name = "../programs/" + base_name + ".bin";

            assemble_file(selected_file, output_file_name);
        }
        else if (choice == 2)
        {
            string bin_directory = "../programs";
            vector<string> bin_files;

            for (const auto &entry : fs::directory_iterator(bin_directory))
            {
                if (entry.is_regular_file() && entry.path().extension() == ".bin")
                    bin_files.push_back(entry.path().string());
            }

            if (bin_files.empty())
            {
                cout << RED << "No binary files found in the directory." << RESET << endl;
                continue;
            }

            cout << GREEN << "Select a file to emulate:" << RESET << endl;
            for (size_t i = 0; i < bin_files.size(); ++i)
                cout << GREEN << i + 1 << ". " << bin_files[i] << RESET << endl;

            cout << endl;
            int file_choice;
            cout << BLUE << "Enter your choice: " << RESET;
            cin >> file_choice;

            if (file_choice < 1 || file_choice > static_cast<int>(bin_files.size()))
            {
                cout << RED << "Invalid choice." << RESET << endl;
                continue;
            }

            string selected_file = bin_files[file_choice - 1];
            emulate_file(selected_file);
        }
        else if (choice == 3)
        {
            cout << CYAN << "Exiting..." << RESET << endl;
            break;
        }
        else
        {
            cout << RED << "Invalid choice." << RESET << endl;
        }
    }
}