#include <iostream>
#include <filesystem>
#include <stack>
#include <string>
#include <chrono>
#include <vector>
#include <iomanip>

using namespace std;
namespace fs = std::filesystem;

// ============================================================
// PHIÊN BẢN 1: ĐỆ QUY
// ============================================================
string recursiveSearch(const fs::path &directory, const string &targetFile, int depth = 0)
{
    if (!fs::exists(directory) || !fs::is_directory(directory))
    {
        return "";
    }

    for (const auto &entry : fs::directory_iterator(directory))
    {
        string currentName = entry.path().filename().string();

        if (fs::is_regular_file(entry) && currentName == targetFile)
        {
            return entry.path().string();
        }

        if (fs::is_directory(entry))
        {
            string result = recursiveSearch(entry.path(), targetFile, depth + 1);
            if (!result.empty())
            {
                return result;
            }
        }
    }

    return "";
}

// ============================================================
// PHIÊN BẢN 2: SỬ DỤNG STACK
// ============================================================
string stackSearch(const fs::path &rootDirectory, const string &targetFile)
{
    if (!fs::exists(rootDirectory) || !fs::is_directory(rootDirectory))
    {
        return "";
    }

    stack<fs::path> dirStack;
    dirStack.push(rootDirectory);

    while (!dirStack.empty())
    {
        fs::path currentDir = dirStack.top();
        dirStack.pop();

        for (const auto &entry : fs::directory_iterator(currentDir))
        {
            string currentName = entry.path().filename().string();

            if (fs::is_regular_file(entry) && currentName == targetFile)
            {
                return entry.path().string();
            }

            if (fs::is_directory(entry))
            {
                dirStack.push(entry.path());
            }
        }
    }

    return "";
}

// ============================================================
// ĐO THỜI GIAN
// ============================================================
void measurePerformance(const fs::path &root, const string &target, int iterations = 10)
{
    cout << "\n=== ĐO HIỆU NĂNG ===" << endl;
    cout << "Thư mục gốc: " << root << endl;
    cout << "File cần tìm: " << target << endl;
    cout << "Số lần lặp: " << iterations << endl;
    cout << "---------------------" << endl;

    // Đo thời gian đệ quy
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; i++)
    {
        string result = recursiveSearch(root, target);
        if (i == 0 && result.empty())
        {
            cout << "Đệ quy: Không tìm thấy file" << endl;
        }
    }
    auto end = chrono::high_resolution_clock::now();
    auto durationRecursive = chrono::duration_cast<chrono::microseconds>(end - start);

    // Đo thời gian Stack
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; i++)
    {
        string result = stackSearch(root, target);
        if (i == 0 && result.empty())
        {
            cout << "Stack: Không tìm thấy file" << endl;
        }
    }
    end = chrono::high_resolution_clock::now();
    auto durationStack = chrono::duration_cast<chrono::microseconds>(end - start);

    // Hiển thị kết quả
    cout << fixed << setprecision(2);
    cout << "Đệ quy: " << (durationRecursive.count() / iterations) << " µs/lần" << endl;
    cout << "Stack  : " << (durationStack.count() / iterations) << " µs/lần" << endl;
    cout << "Tỷ lệ  : " << (double)durationRecursive.count() / durationStack.count() << "x" << endl;
}

// ============================================================
// THỬ NGHIỆM VỚI ĐỘ SÂU LỚN
// ============================================================
void testDeepDirectory(const fs::path &base, int depth)
{
    cout << "\n=== THỬ NGHIỆM VỚI ĐỘ SÂU " << depth << " ===" << endl;

    // Tạo thư mục sâu
    fs::path current = base / "deep_test";
    fs::create_directory(current);

    for (int i = 0; i < depth; i++)
    {
        current = current / ("level_" + to_string(i));
        fs::create_directory(current);
    }

    // Tạo file ở đáy
    fs::path targetFile = current / "secret.txt";
    ofstream(targetFile) << "Found me!";

    cout << "Đã tạo thư mục sâu " << depth << " cấp" << endl;
    cout << "File target tại: " << targetFile << endl;

    // Thử tìm bằng đệ quy
    cout << "\nThử tìm bằng ĐỆ QUY..." << endl;
    try
    {
        auto start = chrono::high_resolution_clock::now();
        string result = recursiveSearch(base / "deep_test", "secret.txt");
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

        if (!result.empty())
        {
            cout << "✓ Tìm thấy: " << result << endl;
            cout << "  Thời gian: " << duration.count() << " ms" << endl;
        }
        else
        {
            cout << "✗ Không tìm thấy!" << endl;
        }
    }
    catch (const exception &e)
    {
        cout << "✗ LỖI: " << e.what() << " (có thể do Stack Overflow)" << endl;
    }

    // Thử tìm bằng Stack
    cout << "\nThử tìm bằng STACK..." << endl;
    try
    {
        auto start = chrono::high_resolution_clock::now();
        string result = stackSearch(base / "deep_test", "secret.txt");
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

        if (!result.empty())
        {
            cout << "✓ Tìm thấy: " << result << endl;
            cout << "  Thời gian: " << duration.count() << " ms" << endl;
        }
        else
        {
            cout << "✗ Không tìm thấy!" << endl;
        }
    }
    catch (const exception &e)
    {
        cout << "✗ LỖI: " << e.what() << endl;
    }

    // Dọn dẹp
    fs::remove_all(base / "deep_test");
}

// ============================================================
// MAIN
// ============================================================
int main()
{
    cout << "====================================================" << endl;
    cout << "  KHỬ ĐỆ QUY BẰNG STACK - BÀI TOÁN DUYỆT THƯ MỤC" << endl;
    cout << "====================================================" << endl;

    // Tạo thư mục test
    fs::path testDir = fs::current_path() / "test_dir";
    if (!fs::exists(testDir))
    {
        // Tạo cấu trúc thư mục test
        fs::create_directories(testDir / "docs" / "sub");
        fs::create_directories(testDir / "data" / "deep");

        ofstream(testDir / "docs" / "a.txt") << "File A";
        ofstream(testDir / "docs" / "sub" / "b.txt") << "File B";
        ofstream(testDir / "data" / "c.txt") << "File C";
        ofstream(testDir / "data" / "deep" / "target.txt") << "Target File";

        cout << "\n✓ Đã tạo thư mục test tại: " << testDir << endl;
    }

    // Test 1: Tìm file
    cout << "\n>>> TEST 1: Tìm file trong thư mục test" << endl;
    string file1 = "target.txt";
    string file2 = "nonexistent.txt";

    auto findAndPrint = [&](const string &filename)
    {
        cout << "Tìm '" << filename << "':" << endl;

        // Đệ quy
        auto start = chrono::high_resolution_clock::now();
        string resultR = recursiveSearch(testDir, filename);
        auto end = chrono::high_resolution_clock::now();
        auto durR = chrono::duration_cast<chrono::microseconds>(end - start);

        // Stack
        start = chrono::high_resolution_clock::now();
        string resultS = stackSearch(testDir, filename);
        end = chrono::high_resolution_clock::now();
        auto durS = chrono::duration_cast<chrono::microseconds>(end - start);

        cout << "  Đệ quy: " << (resultR.empty() ? "Không tìm thấy" : resultR)
             << " (" << durR.count() << " µs)" << endl;
        cout << "  Stack : " << (resultS.empty() ? "Không tìm thấy" : resultS)
             << " (" << durS.count() << " µs)" << endl;
    };

    findAndPrint(file1);
    findAndPrint(file2);

    // Test 2: Đo hiệu năng
    measurePerformance(testDir, file1, 100);

    // Test 3: Thử nghiệm với độ sâu lớn
    vector<int> depths = {10, 100, 500, 1000, 2000};
    for (int d : depths)
    {
        testDeepDirectory(fs::current_path(), d);
    }

    // Dọn dẹp
    cout << "\nDọn dẹp thư mục test..." << endl;
    fs::remove_all(testDir);
    fs::remove_all(fs::current_path() / "deep_test");

    cout << "\n====================================================" << endl;
    cout << "  KẾT THÚC CHƯƠNG TRÌNH" << endl;
    cout << "====================================================" << endl;

    return 0;
}