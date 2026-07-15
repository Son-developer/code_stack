#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

#include "recursive_search.h"
#include "stack_search.h"

int main()
{
#ifdef _WIN32
    // Bat UTF-8 tren console Windows de in tieng Viet co dau dung
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    std::string folder, target;

    std::cout << "Nhap duong dan thu muc can tim (vi du: C:\\Users\\Ten\\Desktop): ";
    std::getline(std::cin, folder);

    std::cout << "Nhap ten file can tim (vi du: anh2.jpg): ";
    std::getline(std::cin, target);

    std::cout << "\n--- Tim bang de quy ---" << std::endl;
    if (!searchRecursive(folder, target))
    {
        std::cout << "File khong ton tai!" << std::endl;
    }

    std::cout << "\n--- Tim bang Stack (iterative) ---" << std::endl;
    if (!searchWithStack(folder, target))
    {
        std::cout << "File khong ton tai!" << std::endl;
    }

    return 0;
}