#include "csv_loader.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Load user dari CSV
std::vector<User> readUserFromCSV(const std::string& filename) {
    std::vector<User> users;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        std::getline(file, line);  // Skip header
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            User user;
            std::string id_str;
            std::getline(ss, id_str, ',');
            user.id = std::stoi(id_str);
            std::getline(ss, user.username, ',');
            std::getline(ss, user.password_hash, ',');
            users.push_back(user);
        }
        file.close();
    } else {
        std::cerr << "Gagal membuka file users.csv.\n";
    }
    return users;
}

// Load event dari CSV
std::vector<Event> readEventFromCSV(const std::string& filename) {
    std::vector<Event> events;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        std::getline(file, line);  // Skip header
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            Event event;
            std::string id_str, quota_str, eventstatus_str, is_paid_str;
            std::getline(ss, id_str, ',');
            event.id = std::stoi(id_str);
            std::getline(ss, event.title, ',');
            std::getline(ss, event.description, ',');
            std::getline(ss, event.start_date, ',');
            std::getline(ss, event.end_date, ',');
            std::getline(ss, event.registration_start, ',');
            std::getline(ss, event.registration_end, ',');
            std::getline(ss, quota_str, ',');
            event.quota = std::stoi(quota_str);
            std::getline(ss, is_paid_str, ',');
            event.is_paid = (is_paid_str == "true" || is_paid_str == "Paid");
            std::getline(ss, event.type, ',');
            std::getline(ss, event.created_at, ',');
            std::getline(ss, eventstatus_str, ',');
            if (eventstatus_str == "BATAL") event.status = BATAL;
            else if (eventstatus_str == "SELESAI") event.status = SELESAI;
            else event.status = AKTIF;
            events.push_back(event);
        }
        file.close();
    } else {
        std::cerr << "Gagal membuka file events.csv.\n";
    }
    return events;
}

// Load registration dari CSV
std::vector<Registration> readRegistrationFromCSV(const std::string& filename) {
    std::vector<Registration> registrations;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        std::getline(file, line);  // Skip header
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            Registration registration;
            std::string status_str, payment_status_str, id_str, event_id_str;
            std::getline(ss, id_str, ',');
            registration.id = std::stoi(id_str);
            std::getline(ss, event_id_str, ',');
            registration.event_id = std::stoi(event_id_str);
            std::getline(ss, registration.full_name, ',');
            std::getline(ss, registration.ttl, ',');
            std::getline(ss, registration.nik, ',');
            std::getline(ss, registration.email, ',');
            std::getline(ss, registration.password_hash, ',');
            std::getline(ss, status_str, ',');
            if (status_str == "PENDING") registration.status = PENDING;
            else if (status_str == "APPROVED") registration.status = APPROVED;
            else if (status_str == "REJECTED") registration.status = REJECTED;
            else registration.status = CANCELLED;
            std::getline(ss, payment_status_str, ',');
            if (payment_status_str == "VERIFIED") registration.payment_status = VERIFIED;
            else if (payment_status_str == "FAILED") registration.payment_status = FAILED;
            else registration.payment_status = UNVERIFIED;
            std::getline(ss, registration.payment_account, ',');
            std::getline(ss, registration.created_at, ',');
            registrations.push_back(registration);
        }
        file.close();
    } else {
        std::cerr << "Gagal membuka file registrations.csv.\n";
    }
    return registrations;
}

// Simpan user ke CSV
void writeUserToCSV(const std::vector<User>& users, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "id,username,password_hash\n";
        for (const auto& user : users) {
            file << user.id << ","
                 << user.username << ","
                 << user.password_hash << "\n";
        }
        file.close();
    } else {
        std::cerr << "Gagal membuka file users.csv untuk menulis.\n";
    }
}

// Simpan event ke CSV
void writeEventToCSV(const std::vector<Event>& events, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "id,title,description,start_date,end_date,registration_start,registration_end,quota,is_paid,type,created_at,status\n";
        for (const auto& event : events) {
            std::string status_str;
            switch (event.status) {
                case BATAL: status_str = "BATAL"; break;
                case SELESAI: status_str = "SELESAI"; break;
                default: status_str = "AKTIF"; break;
            }
            file << event.id << ","
                 << event.title << ","
                 << event.description << ","
                 << event.start_date << ","
                 << event.end_date << ","
                 << event.registration_start << ","
                 << event.registration_end << ","
                 << event.quota << ","
                 << (event.is_paid ? "true" : "false") << ","
                 << event.type << ","
                 << event.created_at << ","
                 << status_str << "\n";
        }
        file.close();
    } else {
        std::cerr << "Gagal membuka file events.csv untuk menulis.\n";
    }
}

// Simpan registration ke CSV
void writeRegistrationToCSV(const std::vector<Registration>& registrations, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "id,event_id,full_name,ttl,nik,email,password,status,payment_status,payment_account,created_at\n";
        for (const auto& registration : registrations) {
            std::string status_str;
            switch (registration.status) {
                case PENDING: status_str = "PENDING"; break;
                case APPROVED: status_str = "APPROVED"; break;
                case REJECTED: status_str = "REJECTED"; break;
                case CANCELLED: status_str = "CANCELLED"; break;
            }
            std::string payment_str;
            switch (registration.payment_status) {
                case VERIFIED: payment_str = "VERIFIED"; break;
                case UNVERIFIED: payment_str = "UNVERIFIED"; break;
                case FAILED: payment_str = "FAILED"; break;
            }
            file << registration.id << ","
                 << registration.event_id << ","
                 << registration.full_name << ","
                 << registration.ttl << ","
                 << registration.nik << ","
                 << registration.email << ","
                 << registration.password_hash << ","
                 << status_str << ","
                 << payment_str << ","
                 << registration.payment_account << ","
                 << registration.created_at << "\n";
        }
        file.close();
    } else {
        std::cerr << "Gagal membuka file registrations.csv untuk menulis.\n";
    }
}

// Tambah user ke CSV (append)
bool appendUser(const User& user, const std::string& filename) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << user.id << ","
             << user.username << ","
             << user.password_hash << "\n";
        file.close();
        return true;
    } else {
        std::cerr << "Gagal membuka file users.csv untuk menambah user.\n";
        return false;
    }
}

// Tambah registration ke CSV (append)
bool appendRegistration(const Registration& registration, const std::string& filename) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        std::string status_str;
        switch (registration.status) {
            case PENDING: status_str = "PENDING"; break;
            case APPROVED: status_str = "APPROVED"; break;
            case REJECTED: status_str = "REJECTED"; break;
            case CANCELLED: status_str = "CANCELLED"; break;
        }
        std::string payment_str;
        switch (registration.payment_status) {
            case VERIFIED: payment_str = "VERIFIED"; break;
            case UNVERIFIED: payment_str = "UNVERIFIED"; break;
            case FAILED: payment_str = "FAILED"; break;
        }
        file << registration.id << ","
             << registration.event_id << ","
             << registration.full_name << ","
             << registration.ttl << ","
             << registration.nik << ","
             << registration.email << ","
             << registration.password_hash << ","
             << status_str << ","
             << payment_str << ","
             << registration.payment_account << ","
             << registration.created_at << "\n";
        file.close();
        return true;
    } else {
        std::cerr << "Gagal membuka file registrations.csv untuk menambah data.\n";
        return false;
    }
}

// Tambah event ke CSV (append)
bool appendEvent(const Event& event, const std::string& filename) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << event.id << ","
             << event.title << ","
             << event.description << ","
             << event.start_date << ","
             << event.end_date << ","
             << event.registration_start << ","
             << event.registration_end << ","
             << event.quota << ","
             << (event.is_paid ? "Paid" : "Free") << ","
             << event.type << ","
             << event.created_at << "\n";
        file.close();
        return true;
    } else {
        std::cerr << "Gagal membuka file events.csv untuk menambah data.\n";
        return false;
    }
}

