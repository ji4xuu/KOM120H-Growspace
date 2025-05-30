#ifndef PARTICIPANT_SERVICE_H
#define PARTICIPANT_SERVICE_H

#include <vector>
#include <string>
#include <unordered_map> // <-- 1. Tambahkan ini untuk Hashing
#include "models.h"
#include "utils/queue.h" // Pastikan ini mengarah ke queue.h baru Anda
#include "other.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <windows.h>
#include <hashing.h>


class ParticipantService {
public:
    ParticipantService(std::vector<Event>& events, std::vector<Registration>& registrations, Queue<Registration>& verifQueue);

    // Mengembalikan daftar event (filter/sort)
    std::vector<Event> listEvents(const std::string& keyword, const std::string& sortBy, bool ascending) const;

    //Mendaftar ke sebuah event
    bool registerToEvent(int eventId);

        
    // Tampilkan status pendaftaran
    Registration* getMyRegistrationStatus(int registrationId);


    Event* findEventById(int eventId) const; 


    int hitungSisaKuota(const Event& ev) const;


    void showEvents(const std::vector<Event> &events, const std::string &keyword, std::string &sortBy, bool &isAscending);

    void getEventDetail(const Event &ev) const;
private:
    std::vector<Event>&        _events;         // reference ke main’s events
    std::vector<Registration>& _registrations;  // reference ke main’s registrations
    Queue<Registration>&       _verifQueue;     // reference ke antrean verifikasi
    std::unordered_map<int, Event*> _eventIndex;
    std::unordered_map<int, Registration*> _registrationIndex;
    void buildIndexes();
};

#endif