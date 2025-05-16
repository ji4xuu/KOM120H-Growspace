#include "other.h"

void clearScreen() {
    std::cout << "\033c";
}

std::string getCurrentDate() {
    std::time_t now = std::time(nullptr);
    std::tm* tm_ptr = std::localtime(&now);

    std::ostringstream oss;
    oss << std::put_time(tm_ptr, "%Y-%m-%d");
    return oss.str();
}

std::string read_line() {
    std::string input;
    std::getline(std::cin >> std::ws, input);
    return input;
}

void pause() {
    std::cout << "\nTekan Enter untuk melanjutkan... ";
    std::cin.ignore();
    std::cin.get();
}
