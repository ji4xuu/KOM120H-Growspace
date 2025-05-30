#include "other.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <regex>

// Bersihkan layar terminal (ANSI escape code, kompatibel Linux/Mac/Windows 10+)
void clearScreen() {
    std::cout << "\033c";
}

// Ambil tanggal hari ini dalam format YYYY-MM-DD
std::string getCurrentDate() {
    std::time_t now = std::time(nullptr);
    std::tm* tm_ptr = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(tm_ptr, "%Y-%m-%d");
    return oss.str();
}

// Membaca satu baris input dari user (mengabaikan whitespace awal)
std::string read_line() {
    std::string input;
    std::getline(std::cin >> std::ws, input);
    return input;
}

// Pause aplikasi sampai user menekan Enter
void pause() {
    std::cout << "\nTekan Enter untuk melanjutkan... ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Validasi format tanggal YYYY-MM-DD
bool isValidDateFormat(const std::string& date_str) {
    static const std::regex pattern(R"(\d{4}-\d{2}-\d{2})");
    return std::regex_match(date_str, pattern);
}