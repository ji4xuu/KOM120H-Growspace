#include <iostream>
#include <csv_loader.h>
#include <admin_menu.h>
#include <participant_menu.h>
#include <participant_service.h>
#include <main_menu.h>
#include <utils/other.h>


int main() {
    void showLogo();
    std::vector<User> users = readUserFromCSV("data/users.csv");
    std::vector<Event> events = readEventFromCSV("data/events.csv");
    std::vector<Registration> registrations = readRegistrationFromCSV("data/registrations.csv");
    Queue<Registration> verifQueue;
    for (auto& r : registrations) {
        if (r.status == PENDING) {
            verifQueue.enqueue(r);
        }
    }

    ParticipantService svc(events, registrations, verifQueue);
    while(true){
        int pilihan = showMainMenu();
        switch (pilihan){
            case 1: {
                int back;
                back = runParticipantMenu(svc);
                writeRegistrationToCSV(registrations, "data/registrations.csv");
                break;
            }
            case 2: {
                break;
            }
            case 0: {
                std::cout << "Terimakasih telah menggunakan Growspace..... See you ^ - ^";
                Sleep(1000);
                return 0;
            }
        }
    }
    
}