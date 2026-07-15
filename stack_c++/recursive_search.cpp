#include "recursive_search.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

// Tuong duong voi search_recursive() trong recursive_search.py
bool searchRecursive(const std::string &folder_path, const std::string &target)
{
    // Duyet qua tung thu trong thu muc
    for (const auto &entry : fs::directory_iterator(folder_path))
    {
        const fs::path &full_path = entry.path();
        std::string name = full_path.filename().string();

        if (entry.is_regular_file() && name == target)
        {
            std::cout << "Tim thay file: " << full_path.string() << std::endl;
            return true;
        }

        if (entry.is_directory())
        {
            // Goi de quy vao thu muc con
            if (searchRecursive(full_path.string(), target))
            {
                return true;
            }
        }
    }
    return false;
}