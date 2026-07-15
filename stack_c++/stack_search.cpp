#include "stack_search.h"
#include <filesystem>
#include <iostream>
#include <stack>

namespace fs = std::filesystem;

// Tuong duong voi search_with_stack() trong stack_search.py
bool searchWithStack(const std::string &folder_path, const std::string &target)
{
    std::stack<std::string> path_stack; // stack chua cac duong dan thu muc can duyet
    path_stack.push(folder_path);

    while (!path_stack.empty())
    {
        std::string current_folder = path_stack.top();
        path_stack.pop();

        for (const auto &entry : fs::directory_iterator(current_folder))
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
                path_stack.push(full_path.string()); // day thu muc con vao stack
            }
        }
    }
    return false;
}