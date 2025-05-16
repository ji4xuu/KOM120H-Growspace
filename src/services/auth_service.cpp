#include "auth_service.h"

bool loginAdmin(const std::string &username, const std::string &password) {
    std::vector<User> users = readUserFromCSV("data/users.csv");  // Memuat data pengguna dari CSV

    // Mencari user dengan email yang cocok
    for (const auto &user : users) {
        if (user.username == username && user.password_hash == password) {
            return true;  // Jika ditemukan, return data user
        }
    }

    return false;  // Return user kosong jika tidak ditemukan
}