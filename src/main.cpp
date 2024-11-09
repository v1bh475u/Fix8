#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

#include "../include/assembler.hpp"
#include "../include/emulator.hpp"

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
        return 1;
    }

    ofstream output_file(output_file_name, ios::binary);

    if (!output_file)
    {
        cerr << "Error: Could not open the file for writing." << endl;
        return 1;
    }

    output_file.write(reinterpret_cast<const char *>(machine_code.data()), machine_code.size());
    output_file.close();
    return 0;
}

int emulate_file(string file_name)
{
    Emulator emulator;
    emulator.load_program(file_name);
    emulator.run();
    return 0;
}

int main()
{
    int choice;
    while (true)
    {
        cout << "Menu:\n";
        cout << "1. Assemble a program\n";
        cout << "2. Emulate a program\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
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
                cout << "No assembly files found in the directory." << endl;
                return 1;
            }

            cout << "Select a file to assemble:" << endl;
            for (size_t i = 0; i < asm_files.size(); ++i)
            {
                cout << i + 1 << ". " << asm_files[i] << endl;
            }

            int file_choice;
            cout << "Enter your choice: ";
            cin >> file_choice;

            if (file_choice < 1 || file_choice > asm_files.size())
            {
                cout << "Invalid choice." << endl;
                return 1;
            }

            string selected_file = asm_files[file_choice - 1];
            string base_name = fs::path(selected_file).stem().string();
            string output_file_name = "../programs/" + base_name + ".bin";

            assemble_file(selected_file, output_file_name);
        }
        else if (choice == 2)
        {
            string asm_directory = "../programs";
            vector<string> asm_files;

            for (const auto &entry : fs::directory_iterator(asm_directory))
            {
                if (entry.is_regular_file() && entry.path().extension() == ".bin")
                {
                    asm_files.push_back(entry.path().string());
                }
            }

            if (asm_files.empty())
            {
                cout << "No assembly files found in the directory." << endl;
                return 1;
            }

            cout << "Select a file to emulate:" << endl;
            for (size_t i = 0; i < asm_files.size(); ++i)
            {
                cout << i + 1 << ". " << asm_files[i] << endl;
            }

            int file_choice;
            cout << "Enter your choice: ";
            cin >> file_choice;

            if (file_choice < 1 || file_choice > asm_files.size())
            {
                cout << "Invalid choice." << endl;
                return 1;
            }

            string selected_file = asm_files[file_choice - 1];
            emulate_file(selected_file);
        }
        else if (choice == 3)
        {
            cout << "Exiting..." << endl;
            break;
        }
        else
        {
            cout << "Invalid choice." << endl;
        }
    }
}