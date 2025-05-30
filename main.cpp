#include <iostream>
#include <csv_loader.h>
#include <admin_menu.h>
#include <participant_menu.h>
#include <participant_service.h>
#include <main_menu.h>
#include <utils/other.h>

int main() {
    showLogo();

    // Load data dari file CSV
    std::vector<User> users = readUserFromCSV("data/users.csv");
    std::vector<Event> events = readEventFromCSV("data/events.csv");
    std::vector<Registration> registrations = readRegistrationFromCSV("data/registrations.csv");

    // Inisialisasi antrean verifikasi
    Queue<Registration> verifQueue;
    for (auto& r : registrations) {
        if (r.status == PENDING) {
            verifQueue.enqueue(r);
        }
    }

    // Inisialisasi service
    ParticipantService participantSvc(events, registrations, verifQueue);
    AdminService adminSvc(events, registrations, verifQueue);

    // Loop utama aplikasi
    while (true) {
        int pilihan = showMainMenu();
        switch (pilihan) {
            case 1: {
                // Menu peserta
                runParticipantMenu(participantSvc);
                writeRegistrationToCSV(registrations, "data/registrations.csv");
                break;
            }
            case 2: {
                // Menu admin
                runAdminMenu(adminSvc);
                writeEventToCSV(events, "data/events.csv");
                writeRegistrationToCSV(registrations, "data/registrations.csv");
                break;
            }
            case 0: {
                std::cout << "Terimakasih telah menggunakan Growspace..... See you ^ - ^";
                Sleep(1000);
                return 0;
            }
            default: {
                std::cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                Sleep(1000);
                break;
            }
        }
    }
}