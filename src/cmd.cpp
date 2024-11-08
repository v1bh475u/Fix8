#include "assembler.hpp"
#include "instruction.hpp"
#include <filesystem>
int main()
{
    std::string path = "../test";
    std::vector<fs::directory_entry> files;
    if (fs::exists(path) && fs::is_directory(path))
    {
        // Iterate through the directory and store files
        for (const auto &entry : fs::directory_iterator(path))
        {
            if (entry.is_regular_file())
            {
                files.push_back(entry);
            }
        }

        // Print files in a pretty fashion
        std::cout << "Files in the test folder:\n";
        for (size_t i = 0; i < files.size(); ++i)
        {
            std::cout << i + 1 << ". " << files[i].path().filename().string() << "\n";
        }

        // Prompt user to choose a file
        std::cout << "Enter the number of the file you want to choose: ";
        size_t choice;
        std::cin >> choice;

        if (choice > 0 && choice <= files.size())
        {
            std::cout << "You chose: " << files[choice - 1].path().filename().string() << "\n";
        }
        else
        {
            std::cout << "Invalid choice.\n";
        }
    }
    else
    {
        std::cout << "The path does not exist or is not a directory.\n";
    }
    return 0;
}