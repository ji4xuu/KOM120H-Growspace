#ifndef MODELS_H
#define MODELS_H
#include <string>
#include <vector>
#include <ctime>

enum PaymentStatus {
    UNVERIFIED,
    VERIFIED,
    FAILED
};

enum RegistStatus {
    PENDING,
    APPROVED,
    REJECTED,
    CANCELLED
};

enum ActionType {
    CREATE_EVENT,
    //EDIT_EVENT,
    DELETE_EVENT,
    VERIFY_REGISTRATION
};

enum EventStatus {
    AKTIF,
    BATAL,
    SELESAI
};

struct User {
    int id;
    std::string username;
    std::string password_hash;
};

struct Event {
    int id;
    std::string title;
    std::string description;
    std::string start_date;
    std::string end_date;
    std::string registration_start;
    std::string registration_end;
    int quota;
    bool is_paid;
    std::string type;
    std::string created_at;
    EventStatus status;
};

struct Registration {
    int id;
    int event_id;
    std::string full_name;
    std::string ttl;
    std::string nik;
    std::string email;
    std::string password_hash;
    RegistStatus status; 
    PaymentStatus payment_status;
    std::string payment_account;
    std::string created_at;
};


struct UndoRecord {
    ActionType action;
    int        target_id;
    std::string pre_state;    // misal JSON atau CSV line
    std::time_t timestamp;

    // Konstruktor helper
    UndoRecord(ActionType a, int id, const std::string& state)
      : action(a), target_id(id), pre_state(state), timestamp(std::time(nullptr))
    {}
};


#endif // MODELS_H