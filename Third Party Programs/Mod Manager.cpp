#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <windows.h>

namespace fs = std::filesystem;

int main() {
    const int maxMods = 12;
    int count = 0;
    bool missing = false;

    if (!fs::exists("Unknown")) {
        fs::create_directory("Unknown");
    }

    
    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        if (entry.is_directory()) {
            std::string folderName = entry.path().filename().string();

            if (folderName == "Unknown") continue;

            std::string settingsPath = entry.path().string() + "\\Settings.ini";

            if (fs::exists(settingsPath)) {
                count++;
                if (count <= maxMods) {
                    std::string newName = "Mod" + std::to_string(count);

                    std::cout << "Renaming \"" << folderName << "\" to \"" << newName << "\"\n";

                    fs::rename(entry.path(), entry.path().parent_path() / newName);
                }
            } else {
                std::cout << "Moving folder \"" << folderName << "\" to Unknown (no Settings.ini found)...\n";
                fs::rename(entry.path(), fs::current_path() / "Unknown" / folderName);
            }
        }
    }

    //
    std::cout << "\nChecking sequence...\n";
    int last = 0;

    for (int i = 1; i <= maxMods; i++) {
        std::string modName = "Mod" + std::to_string(i);
        if (fs::exists(modName)) {
            if (i > 1 && !fs::exists("Mod" + std::to_string(i - 1))) {
                std::cout << "Error: Found " << modName << " but Mod" << (i - 1) << " is missing!\n";
                missing = true;
                break;
            }
            last = i;
        }
    }

    if (!missing) {
        std::cout << "All mods are in correct sequence.\n";
    } else {
        std::cout << "Sequence error detected!\n";
    }

    system("pause");
    return 0;
}
