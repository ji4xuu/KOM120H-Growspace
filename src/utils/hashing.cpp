#include "hashing.h"
#include <functional>
#include <sstream>
#include <iomanip>

// Hash password menggunakan std::hash dan hasilkan string heksadesimal
std::string hashPassword(const std::string& password) {
    std::hash<std::string> hasher;
    size_t hashed_value = hasher(password);

    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << hashed_value;
    return ss.str();
}