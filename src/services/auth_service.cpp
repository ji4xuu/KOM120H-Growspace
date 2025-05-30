#include "auth_service.h"

// Mengembalikan true jika username dan password hash cocok dengan data admin
bool loginAdmin(const std::string& username, const std::string& password) {
    const auto users = readUserFromCSV("data/users.csv");
    for (const auto& user : users) {
        if (user.username == username && user.password_hash == password) {
            return true;
        }
    }
    return false;
}