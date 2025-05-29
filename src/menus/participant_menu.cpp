#include "participant_menu.h"
#include "main_menu.h"

void showMenu() {
    std::cout << std::string(6, ' ') << std::string(50, '-') << " Dashboard Peserta " << std::string(50, '-') << '\n';
    std::cout << R"(
          1. List Event yang Tersedia       2. Daftar ke Event         3. Cek Status Pendaftaran         0. Kembali
)" << '\n' << '\n';
    std::cout << "Masukkan pilihan anda [0 - 3] : ";
}

static void handleListEvents(ParticipantService& svc)
{
    std::string sortBy;
    std::string keyword;
    bool ascending;

    while(true){
        
        std::cout << "Masukkan kata kunci untuk mencari event (- untuk menampilkan semua): ";
        keyword = read_line();
        std::cout << "Masukkan nama kolom untuk pengurutan (contoh: title, tanggal_mulai, tanggal_selesai). Isi - untuk default id : ";
        std::string sortByInput = read_line();
    
        sortBy = "id"; // default
        if (!sortByInput.empty()) {
            if (
                sortByInput == "id" ||
                sortByInput == "title" ||
                sortByInput == "tanggal_mulai" ||
                sortByInput == "tanggal_selesai" ||
                sortByInput == "tanggal_mulai_daftar" ||
                sortByInput == "tanggal_selesai_daftar" ||
                sortByInput == "sisa_kuota" ||
                sortByInput == "-"
            ) {
                sortBy = sortByInput;
            } else {
                std::cout << "Input kolom tidak dikenali. Menggunakan default: id.\n";
            }
        }
    
        std::cout << "Masukkan urutan pengurutan (asc untuk menaik, desc untuk menurun). : ";
        std::string sortOrder = read_line();
        ascending = true; // default
        if (!sortOrder.empty()) {
            if (sortOrder == "desc" || sortOrder == "DESC"){
                ascending = false;
                break;
            } else if(sortOrder == "asc" || sortOrder == "ASC"){
                break;
            }
            else{
                std::cout << "Input kolom tidak dikenali. Menggunakan default: asc.\n";
                break;
            }
        }
    }
    
    

    auto events = svc.listEvents(keyword, sortBy, ascending);
    if (events.empty()) {
        std::cout << "Tidak ada event ditemukan.\n";
        Sleep(2000);

    } else {
        int pilihan;
        while(true){
            clearScreen();
            std::cout << std::string(10, ' ') << std::string(43, '-') << " List Event yang Tersedia " << std::string(40, '-') << '\n' << '\n';
            svc.showEvents(events, keyword, sortBy, ascending);

            std::cout << R"(
                          1. Lihat Detail Event                             0. Kembali)" << '\n' << '\n';
            std::cout << "Masukkan pilihan anda [0 - 1] : ";
            std::cin >> pilihan;
            if(pilihan == 1){
                int eventid;
                std::cout << "Masukkan event id : ";
                std::cin >> eventid;
                Event* event = svc.findEventById(eventid);
                if (event != nullptr) { 
                    std::cout << "\nBerikut detail Event dengan id : " << eventid << '\n';
                    svc.getEventDetail(*event); 
                } else {
                    std::cout << "Event dengan ID " << eventid << " tidak ditemukan.\n";
                }
            }
            else if(pilihan == 0){
                std::cout << "Kembali ke Dashboard Peserta..." << '\n';
                Sleep(2000);
                break;
            }
            else{
                std::cout << "Input yang diberikan tidak valid." << '\n';
            }
            pause();
        }
        
        
    }
}


static void handleRegister(ParticipantService& svc)
{
    std::cout << "Masukkan Event ID: ";
    int eventId;
    std::cin >> eventId;

    // Validasi input
    if (std::cin.fail()) {
        std::cin.clear(); // Menghapus flag error
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Mengabaikan input yang tersisa
        std::cout << "Input tidak valid. Silakan masukkan angka.\n";
        return;
    }

    // Cari event berdasarkan ID
    const Event* event = svc.findEventById(eventId);
    if (event == nullptr) {
        std::cout << "Event dengan ID " << eventId << " tidak ditemukan.\n";
        return;
    }

    // Tampilkan informasi event
    std::cout << "\nInformasi Event:\n";
    svc.getEventDetail(*event);

    // Periksa kuota
    if (event->quota <= 0) {
        std::cout << "Maaf, kuota untuk event ini telah habis.\n";
        return;
    }

    // Konfirmasi pendaftaran
    std::cout << "Apakah Anda ingin mendaftar ke event ini? (y/n): ";
    char confirm;
    std::cin >> confirm;
    if (confirm != 'y' && confirm != 'Y') {
        std::cout << "Pendaftaran dibatalkan.\n";
        return;
    }

    // Proses pendaftaran
    bool result = svc.registerToEvent(eventId);
    if (result) {
        std::cout << "Pendaftaran gagal: " << "\n";
        std::cout << "Kembali ke Dashboard Peserta..." << '\n';
        Sleep(2000);
    } else {
        pause();
        std::cout << "Kembali ke Dashboard Peserta..." << '\n';
        Sleep(2000);
    }
}

static void handleCheckStatus(ParticipantService& svc){
    int reg_id;
    std::string email, password;
    constexpr int LBL_W = 18;  
    clearScreen();

    std::cout << "Masukkan ID Registrasi  : ";
    std::cin >> reg_id;
    Registration* reg = svc.getMyRegistrationStatus(reg_id);
    std::cout << "Masukkan Email          : ";
    std::cin >> email;
    std::cout << "Masukkan Password       : ";
    std::cin >> password;
    if(reg->id == -1){
        std::cout << "Belum ada pendaftaran.\n";
    } else if (reg->email != email) {
        std::cout << "\nEmail atau Password salah.\n";
        pause();
        return;
    } else if (reg->password_hash != password) {
        std::cout << "\nEmail atau Password salah.\n";
        pause();
        return;
    }
    else {
        while(true){
            std::string status_str;
            switch (reg->status) {
                case PENDING: status_str = "Pending"; break;
                case APPROVED: status_str = "Approved"; break;
                case REJECTED: status_str = "Rejected"; break;
                case CANCELLED: status_str = "Cancelled"; break;
            }

            std::string payment_str;
            switch (reg->payment_status) {
                case VERIFIED: payment_str = "Verified"; break;
                case UNVERIFIED: payment_str = "Unverified"; break;
                case FAILED: payment_str = "Failed"; break;
            }
            clearScreen();
            std::cout << "============ Detail Status Registrasi ============\n\n";
            std::cout << std::left << std::setw(LBL_W) << "ID Registrasi"   << ": " << reg->id            << "\n";
            std::cout << std::left << std::setw(LBL_W) << "Event ID"         << ": " << reg->event_id     << "\n";
            std::cout << std::left << std::setw(LBL_W) << "Nama Lengkap"    << ": " << reg->full_name    << "\n";
            std::cout << std::left << std::setw(LBL_W) << "Tanggal Lahir"   << ": " << reg->ttl           << "\n";
            std::cout << std::left << std::setw(LBL_W) << "NIK"             << ": " << reg->nik           << "\n";
            std::cout << std::left << std::setw(LBL_W) << "Email"           << ": " << reg->email         << "\n";
            std::cout << std::left << std::setw(LBL_W) << "Status Pendaftaran" << ": " << status_str      << "\n";
            std::cout << std::left << std::setw(LBL_W) << "Status Pembayaran"  << ": " << payment_str << "\n";
            std::cout << std::left << std::setw(LBL_W) << "Dibuat pada"      << ": " << reg->created_at    << "\n\n";
            
            std::cout << R"(         1. Batalkan Pendaftaran Event               0. Kembali
            )" << '\n' << '\n';
            std::cout << "Masukkan pilihan Anda [0 - 1] : ";
            int pilihan;
            std::cin >> pilihan;
            if(pilihan == 1){
                std::cout << "Apakah anda ingin membatalkan pendaftaran ini? (y/n): ";
                char confirm;
                std::cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    reg->status = CANCELLED;
                    std::cout << "Pendaftaran berhasil dibatalkan.\n";
                }
            }
            else if(pilihan == 0){
                pause();
                std::cout << "Kembali ke Dashboard Peserta..." << '\n';
                Sleep(1000);
                break;
            }
            else{
                std::cout << "Pilihan tidak valid.\n";
                continue;
            }
        }

    }

}

int runParticipantMenu(ParticipantService& svc) {
    int choice;
    do {
        clearScreen();
        showLogo();
        showMenu();
        std::cin >> choice;
        switch (choice) {
            case 1: handleListEvents(svc);    break;
            case 2: handleRegister(svc);      break;
            case 3: handleCheckStatus(svc);   break;
            case 0: return 0;
            default:
                std::cout << "Pilihan tidak valid.\n";
        }
    } while (choice != 0);
    return 0;
}