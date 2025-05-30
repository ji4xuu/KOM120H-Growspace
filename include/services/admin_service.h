#ifndef ADMIN_SERVICE_H
#define ADMIN_SERVICE_H

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include "models.h"
#include "utils/queue.h"
#include "utils/stack.h"
#include <unordered_map>

class AdminService {
public:
    // Constructor
    AdminService(
        std::vector<Event>& events, 
        std::vector<Registration>& registrations, 
        Queue<Registration>& verifQueue
    );

    // Manajemen Verifikasi (Queue)
    void viewVerificationQueue() const;
    bool processNextRegistration(bool approve);

    // Manajemen Event (CRUD)
    bool createEvent(
    const std::string& title, 
    const std::string& description, 
    const std::string& start_date, 
    const std::string& end_date,
    const std::string& registration_start,
    const std::string& registration_end,
    int quota,
    bool is_paid,
    const std::string& type
);
    bool deleteEvent(int eventId);
    // bool updateEvent(int eventId, const std::string& newTitle, const std::string& newDesc, int newQuota);
    // (Fungsi updateEvent bisa ditambahkan di sini jika perlu)

    std::string peekLastActionDescription() const;
    
    // Fitur Undo (Stack)
    bool undoLastAction();
    void listAllEventsForAdmin() const;
    Event* findEventById(int eventId);


private:

    std::vector<Event>&        _events;
    std::vector<Registration>& _registrations;
    Queue<Registration>&       _verifQueue;
    Stack<UndoRecord> _actionStack;

    
    Registration* findRegistrationById(int registrationId);
    
    // Fungsi untuk mengubah objek Event menjadi string dan sebaliknya, untuk disimpan di UndoRecord
    std::string serializeEvent(const Event& event) const;
    Event deserializeEvent(const std::string& eventString) const;

    std::unordered_map<int, Event*> _adminEventIndex;
    std::unordered_map<int, Registration*> _adminRegistrationIndex;
    void buildAdminIndexes(); 
};

#endif