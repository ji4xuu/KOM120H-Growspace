#include "csv_loader.h"

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
        std::cerr << "Terjadi kesalahan saat membuka file users.csv untuk membaca data.\n";
    }
    return users;
}

std::vector<Event> readEventFromCSV(const std::string& filename) {
    std::vector<Event> events;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        std::getline(file, line);  // Skip header
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            Event event;
            std::string event_status;
            std::string id_str;
            std::string quota_str;
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
            std::string is_paid;
            std::getline(ss, is_paid, ',');
            event.is_paid = (is_paid == "true");
            std::getline(ss, event.type, ',');
            std::getline(ss, event.created_at, ',');
            events.push_back(event);
        }
        file.close();
    } else {
        std::cerr << "Terjadi kesalahan saat membuka file events.csv untuk membaca data.\n";
    }
    return events;
}

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
        std::cerr << "Terjadi kesalahan saat membuka file registrations.csv untuk membaca data.\n";
    }
    return registrations;
}


void writeUserToCSV(const std::vector<User>& users, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "id,username,password_hash\n";  // Header CSV
        for (const auto& user : users) {
            file << user.id << ","
                 << user.username << ","
                 << user.password_hash << "\n";
        }
        file.close();
    } else {
        std::cerr << "Terjadi kesalahan saat membuka file users.csv untuk menulis.\n";
    }
}

void writeEventToCSV(const std::vector<Event>& events, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "id,title,description,start_date,end_date,registration_start,registration_end,quota,is_paid,type,created_at\n";  // Header CSV
        for (const auto& event : events) {
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
        }
        file.close();
    } else {
        std::cerr << "Terjadi kesalahan saat membuka file events.csv untuk menulis.\n";
    }
}

void writeRegistrationToCSV(const std::vector<Registration>& registrations, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "id,event_id,full_name,ttl,nik,email,status,payment_status,created_at\n";  // Header CSV
        for (const auto& registration : registrations) {
            std::string status_str;
            switch (registration.status) {
                case PENDING: status_str = "Pending"; break;
                case APPROVED: status_str = "Approved"; break;
                case REJECTED: status_str = "Rejected"; break;
                case CANCELLED: status_str = "Cancelled"; break;
            }

            std::string payment_str;
            switch (registration.payment_status) {
                case VERIFIED: payment_str = "Verified"; break;
                case UNVERIFIED: payment_str = "Unverified"; break;
                case FAILED: payment_str = "Failed"; break;
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
                 << registration.created_at << "\n";
        }
        file.close();
    } else {
        std::cerr << "Terjadi kesalahan saat membuka file registrations.csv untuk menulis.\n";
    }
}


bool appendUser(const User& user, const std::string& filename) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << user.id << ","
             << user.username << ","
             << user.password_hash << "\n";
        file.close();
        return true;
    } else {
        std::cerr << "Terjadi kesalahan saat membuka file users.csv untuk menambahkan user.\n";
        return false;
    }
}

bool appendRegistration(const Registration& registration, const std::string& filename) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        std::string status_str;
        switch (registration.status) {
            case PENDING: status_str = "Pending"; break;
            case APPROVED: status_str = "Approved"; break;
            case REJECTED: status_str = "Rejected"; break;
            case CANCELLED: status_str = "Cancelled"; break;
        }

        std::string payment_str;
        switch (registration.payment_status) {
            case VERIFIED: payment_str = "Verified"; break;
            case UNVERIFIED: payment_str = "Unverified"; break;
            case FAILED: payment_str = "Failed"; break;
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
             << registration.created_at << "\n";
        file.close();
        return true;
    } else {
        std::cerr << "Terjadi kesalahan saat membuka file registrations.csv untuk menambahkan data.\n";
        return false;
    }
}


bool appendEvent(const Event& event, const std::string& filename){
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()){
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
        std::cerr << "Terjadi kesalahan saat membuka file events.csv untuk menambahkan data.\n";
        return false;
    }
}

