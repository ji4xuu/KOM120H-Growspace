#include "hashing.h"
#include <functional> // Diperlukan untuk std::hash
#include <sstream>    // Diperlukan untuk std::stringstream
#include <iomanip>    // Diperlukan untuk std::hex dan std::setfill

// Implementasi fungsi hashPassword
std::string hashPassword(const std::string& password) {
    // 1. Buat objek hasher dari C++ Standard Library
    std::hash<std::string> hasher;

    // 2. Lakukan hashing pada password. Hasilnya adalah angka (size_t)
    size_t hashed_value = hasher(password);

    // 3. Ubah hasil hash (angka) menjadi format string heksadesimal
    //    agar panjangnya konsisten dan aman untuk disimpan di CSV.
    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << hashed_value;
    
    return ss.str();
}