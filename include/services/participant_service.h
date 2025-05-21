#ifndef PARTICIPANT_SERVICE_H
#define PARTICIPANT_SERVICE_H


#include <vector>
#include "models.h"
#include "utils/queue.h"
#include "queue.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "other.h"
#include <windows.h>

class ParticipantService {
public:
    ParticipantService(std::vector<Event>& events, std::vector<Registration>& registrations, Queue<Registration>& verifQueue);

    // Mengembalikan daftar event (filter/sort)
    std::vector<Event> listEvents(const std::string& keyword, const std::string& sortBy, bool ascending) const;

    //Mendaftar ke sebuah event
    bool registerToEvent(int eventId);

        
    // Tampilkan status pendaftaran
    Registration* getMyRegistrationStatus(int registrationId);


    Event findEventId(int eventId) const; 


    int hitungSisaKuota(const Event& ev) const;


    void showEvents(const std::vector<Event> &events, const std::string &keyword, std::string &sortBy, bool &isAscending);

    void getEventDetail(const Event &ev) const;
private:
    std::vector<Event>&        _events;         // reference ke main’s events
    std::vector<Registration>& _registrations;  // reference ke main’s registrations
    Queue<Registration>&       _verifQueue;     // reference ke antrean verifikasi
};

#endif