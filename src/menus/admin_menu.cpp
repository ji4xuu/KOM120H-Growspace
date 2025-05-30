#include "admin_menu.h"    // Header untuk menu ini
#include "admin_service.h" // Untuk menggunakan AdminService
#include "main_menu.h"     // Untuk showLogo()
#include "utils/other.h"   // Untuk clearScreen(), read_line(), pause()
#include <iostream>
#include <string>
#include <limits>          // Untuk std::numeric_limits

// Fungsi untuk menampilkan opsi menu admin (mirip showMenu() di participant_menu)
static void showAdminMenu() {
    std::cout << std::string(6, ' ') << std::string(51, '-') << " Dashboard Admin " << std::string(51, '-') << '\n';
    std::cout << R"(
            1. Verifikasi Pendaftaran                   2. Manajemen Event                    0. Logout
)" << '\n' << '\n';
    std::cout << "Masukkan pilihan anda [0 - 2] : ";
}

static void handleAdminCreateEvent(AdminService& service) {
    clearScreen();
    showLogo();
    std::cout << std::string(6, ' ') << std::string(50, '-') << " Buat Event Baru " << std::string(52, '-') << '\n';
    
    std::string title, description, start_date, end_date, registration_start, registration_end, type_str;
    int quota_int;
    char is_paid_char;
    bool is_paid_bool;

    std::cout << "Judul Event                                : ";
    while (true) {
        title = read_line();
        if (title.empty()) {
            std::cout << "Judul tidak boleh kosong. Masukkan lagi: ";
        } else {
            break;
        }
    }

    std::cout << "Deskripsi Event (pisahkan baris dengan '|'): ";
    while (true) {
        description = read_line();
        if (description.empty()) {
            std::cout << "Deskripsi tidak boleh kosong. Masukkan lagi: ";
        } else {
            break;
        }
    }
    
    std::cout << "Tanggal Mulai Event (YYYY-MM-DD)           : ";
    while (true) {
        start_date = read_line();
        if (start_date.empty()) {
            std::cout << "Tanggal mulai tidak boleh kosong. Masukkan lagi (YYYY-MM-DD): ";
            continue;
        }
        if (!isValidDateFormat(start_date)) { 
            std::cout << "Format tanggal salah (YYYY-MM-DD). Masukkan lagi: ";
            continue;
        }

        break;
    }

    std::cout << "Tanggal Selesai Event (YYYY-MM-DD)         : ";
    while (true) {
        end_date = read_line();
        if (end_date.empty()) {
            std::cout << "Tanggal selesai tidak boleh kosong. Masukkan lagi (YYYY-MM-DD): ";
            continue;
        }
        if (!isValidDateFormat(end_date)) { // Panggil fungsi validasi format Anda
            std::cout << "Format Tanggal Selesai salah (YYYY-MM-DD). Masukkan lagi: ";
            continue;
        }
        // VALIDASI LOGIS IN-PLACE: end_date harus setelah atau sama dengan start_date
        // Perbandingan string "YYYY-MM-DD" bisa bekerja untuk urutan dasar
        if (end_date < start_date) { 
            std::cout << "Tanggal Selesai tidak boleh sebelum Tanggal Mulai (" << start_date << "). Masukkan lagi: ";
            continue;
        }
        break;
    }

    std::cout << "Tgl Mulai Pendaftaran (YYYY-MM-DD)         : ";
    
    while (true) {
        registration_start = read_line();
        if (registration_start.empty()) {
            std::cout << "Tgl mulai pendaftaran tidak boleh kosong. Masukkan lagi (YYYY-MM-DD): ";
            continue;
        }
        if (!isValidDateFormat(registration_start)) { // Panggil fungsi validasi format Anda
            std::cout << "Format Tgl Mulai Pendaftaran salah (YYYY-MM-DD). Masukkan lagi: ";
            continue;
        }
        // VALIDASI LOGIS IN-PLACE: Tgl mulai pendaftaran harus sebelum atau sama dengan tgl mulai event
        if (registration_start > start_date) {
            std::cout << "Tgl Mulai Pendaftaran tidak boleh setelah Tanggal Mulai Event (" << start_date << "). Masukkan lagi: ";
            continue;
        }
        break;
    }

    std::cout << "Tgl Selesai Pendaftaran (YYYY-MM-DD)       : ";
    
    while (true) {
        registration_end = read_line();
        if (registration_end.empty()) {
            std::cout << "Tgl selesai pendaftaran tidak boleh kosong. Masukkan lagi (YYYY-MM-DD): ";
            continue;
        }
        if (!isValidDateFormat(registration_end)) { // Panggil fungsi validasi format Anda
            std::cout << "Format Tgl Selesai Pendaftaran salah (YYYY-MM-DD). Masukkan lagi: ";
            continue;
        }
        // VALIDASI LOGIS IN-PLACE:
        if (registration_end < registration_start) {
            std::cout << "Tgl Selesai Pendaftaran tidak boleh sebelum Tgl Mulai Pendaftaran (" << registration_start << "). Masukkan lagi: ";
            continue;
        }
        if (registration_end > end_date) { // Sebaiknya pendaftaran ditutup sebelum atau saat event berakhir
            std::cout << "Tgl Selesai Pendaftaran tidak boleh setelah Tanggal Selesai Event (" << end_date << "). Masukkan lagi: ";
            continue;
        }
         if (registration_end > start_date) { // Paling logis, pendaftaran ditutup sebelum atau saat event dimulai
            std::cout << "Peringatan: Tgl Selesai Pendaftaran (" << registration_end <<") melewati Tanggal Mulai Event (" << start_date << ").\n";
            // Anda bisa memutuskan apakah ini error atau hanya peringatan
        }
        break;
    }

    std::cout << "Kuota Peserta                              : ";
    while (!(std::cin >> quota_int) || quota_int < 0) {
        std::cout << "Input kuota tidak valid. Masukkan angka positif: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "Apakah event berbayar? (y/n)               : ";
    is_paid_char = read_line()[0];
    is_paid_bool = (is_paid_char == 'y' || is_paid_char == 'Y');

    std::cout << "Tipe Event (contoh: Workshop, Seminar)     : "; type_str = read_line();

    // Panggil service dengan semua parameter
    service.createEvent(title, description, start_date, end_date, registration_start, registration_end, quota_int, is_paid_bool, type_str);
    pause();
}

// Handler untuk menghapus event
static void handleAdminDeleteEvent(AdminService& service) {
    clearScreen();
    showLogo();
    std::cout << std::string(6, ' ') << std::string(50, '-') << " Hapus Event " << std::string(57, '-') << '\n';
    
    int eventId;
    std::cout << "Masukkan ID Event yang akan dihapus: ";
    while (!(std::cin >> eventId)) {
        std::cout << "Input ID tidak valid. Masukkan angka: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Event* eventToDelete = service.findEventById(eventId); // Panggil findEventById dari service

    if (eventToDelete == nullptr) {
        std::cout << "\nEvent dengan ID " << eventId << " tidak ditemukan.\n";
    } else {
        std::cout << std::string(35, ' ') << std::string(61, '=') << '\n';
        std::cout << std::string(50, ' ') << " Detail Event yang Akan Dihapus \n";
        std::cout << std::string(35, ' ') << std::string(61, '=') << '\n';
        
        // Menggunakan std::left dan std::setw untuk merapikan tampilan
        const int labelWidth = 22; // Lebar untuk kolom label
        
        std::cout << std::string(41, ' ') << std::left << std::setw(labelWidth) << "ID Event" << ": " << eventToDelete->id << std::endl;
        std::cout << std::string(41, ' ') << std::left << std::setw(labelWidth) << "Judul" << ": " << eventToDelete->title << std::endl;
        std::cout << std::string(41, ' ') << std::left << std::setw(labelWidth) << "Tanggal Pendaftaran" << ": " << eventToDelete->start_date << " - " << eventToDelete->end_date << std::endl;
        std::cout << std::string(41, ' ') << std::left << std::setw(labelWidth) << "Tanggal Pelaksanaan" << ": " << eventToDelete->registration_start << " - " << eventToDelete->registration_end << std::endl;
        std::cout << std::string(41, ' ') << std::left << std::setw(labelWidth) << "Tipe" << ": " << eventToDelete->type << std::endl;
        std::cout << std::string(41, ' ') << std::left << std::setw(labelWidth) << "Kuota" << ": " << eventToDelete->quota << std::endl;
        std::cout << std::string(35, ' ') << std::string(61, '=') << '\n';

        char confirm;
        std::cout << "\nApakah Anda yakin ingin menghapus event '" << eventToDelete->title << "' (ID: " << eventId << ")? (y/n): ";
        confirm = read_line()[0]; // Ambil karakter pertama dari input

        if (confirm == 'y' || confirm == 'Y') {
            service.deleteEvent(eventId); // Panggil fungsi delete di service
                                          // Pesan sukses/gagal sudah dari service.deleteEvent()
        } else {
            std::cout << "\nPenghapusan event dibatalkan.\n";
        }
    }
    pause();
}
/*
static void handleAdminUpdateEvent(AdminService& service) {
    clearScreen();
    showLogo(); // Jika Anda menggunakan ini
    std::cout << std::string(6, ' ') << std::string(50, '-') << " Update Event " << std::string(55, '-') << '\n';
    
    int eventId;
    std::cout << "Masukkan ID Event yang akan diupdate: ";
    while (!(std::cin >> eventId) || eventId <= 0) {
        std::cout << "Input ID tidak valid. Masukkan angka positif: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Event* currentEvent = service.findEventById(eventId); // Anda perlu membuat findEventById di AdminService menjadi public atau punya fungsi display
    if (!currentEvent) {
        std::cout << "Event dengan ID " << eventId << " tidak ditemukan.\n";
        pause();
        return;
    }
    std::cout << "\n--- Detail Event Saat Ini (ID: " << currentEvent->id << ") ---\n";
    std::cout << "Judul    : " << currentEvent->title << std::endl;
    std::cout << "Deskripsi: " << currentEvent->description << std::endl;
    // std::cout << "Kuota    : " << currentEvent->quota << std::endl;
    // ... (tampilkan field lain jika perlu)

    std::string newTitle, newDesc;
    int newQuota;

    std::cout << "\nMasukkan Data Baru (kosongkan/isi -1 jika tidak ingin mengubah field tertentu,\nnamun untuk implementasi service saat ini, semua field ini akan di-set):\n";
    
    std::cout << "Judul Event Baru              : "; newTitle = read_line();
    std::cout << "Deskripsi Event Baru          : "; newDesc = read_line();
    std::cout << "Kuota Peserta Baru (-1 skip): ";
    while (!(std::cin >> newQuota)) {
        std::cout << "Input kuota tidak valid. Masukkan angka: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Jika Anda ingin handle "skip" dengan lebih baik, Anda perlu mengambil
    // data event lama dulu, lalu hanya update field yang diisi pengguna.
    // Untuk sekarang, implementasi AdminService kita akan mengupdate semua field yang dipassing.
    // Jadi, jika pengguna tidak ingin mengubah, mereka harus memasukkan nilai lama.
    // Alternatif: Modifikasi service.updateEvent untuk menerima, misalnya, std::optional<std::string>

    if (service.updateEvent(eventId, newTitle, newDesc, newQuota)) {
        // Pesan sukses sudah dari service
    } else {
        // Pesan gagal sudah dari service
    }
    pause();
}

*/

// Handler untuk proses verifikasi
static void handleAdminProcessRegistration(AdminService& service) {
    clearScreen();
    showLogo();
    std::cout << std::string(6, ' ') << std::string(45, '-') << " Proses Verifikasi Pendaftaran " << std::string(45, '-') << '\n';
    
    service.viewVerificationQueue(); // Tampilkan antrean dulu

    // Kita perlu cara aman untuk tahu jika queue kosong dari service
    // Untuk sekarang, asumsikan service.viewVerificationQueue() memberi info jika kosong
    // Atau kita bisa tambahkan: if (service.isVerificationQueueEmpty()) { pause(); return; }

    char pilihan;
    std::cout << "\nProses pendaftar berikutnya? (y/n): ";
    pilihan = read_line()[0]; // Ambil karakter pertama

    if (pilihan == 'y' || pilihan == 'Y') {
        char setuju;
        std::cout << "Setujui (approve) pendaftaran ini? (y/n): ";
        setuju = read_line()[0];
        service.processNextRegistration(setuju == 'y' || setuju == 'Y');
    } else {
        std::cout << "Proses dibatalkan.\n";
    }
    pause();
}

static void handleUndoActionInteraktif(AdminService& service) {
    clearScreen();
    showLogo(); // Jika Anda menggunakan ini
    std::cout << std::string(6, ' ') << std::string(50, '-') << " Undo Aksi Terakhir " << std::string(50, '-') << '\n';

    std::string actionDesc = service.peekLastActionDescription();
    std::cout << actionDesc << std::endl;

    if (actionDesc == "Tidak ada aksi untuk di-undo." || actionDesc == "Aksi terakhir tidak dikenali.") {
        pause();
        return;
    }

    char confirm;
    std::cout << "\nApakah Anda yakin ingin membatalkan aksi ini? (y/n): ";
    confirm = read_line()[0];

    if (confirm == 'y' || confirm == 'Y') {
        if (service.undoLastAction()) {
            // Pesan sukses sudah ada di dalam undoLastAction (misalnya "[UNDO] Verifikasi...")
        } else {
            // Ini seharusnya tidak terjadi jika peek berhasil, tapi sebagai safety net
            std::cout << "Gagal melakukan undo.\n";
        }
    } else {
        std::cout << "Undo dibatalkan.\n";
    }
    pause();
}

static void runVerificationSubMenu(AdminService& service) {
    int choice;
    do {
        clearScreen();
        showLogo();
        std::cout << std::string(6, ' ') << std::string(45, '-') << " Proses Verifikasi Pendaftaran " << std::string(45, '-') << '\n';
        service.viewVerificationQueue(); // Menampilkan pendaftar terdepan

        std::cout << std::string(8, ' ') << std::string(45, '-') << " Pilihan Aksi Verifikasi " << std::string(45, '-') << '\n';
        std::cout << R"(
            1. Proses Pendaftar Berikutnya              2. Undo Aksi Terakhir                  0. Kembali
)" << '\n' << '\n';
        std::cout << "Masukkan pilihan Anda: ";

        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -99; // Pilihan tidak valid
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1: {
                // Cek dulu apakah antrean kosong (sebaiknya ada fungsi di service)
                // if (service.isVerificationQueueEmpty()) { // Anda perlu buat fungsi ini di AdminService
                //    std::cout << "Antrean sudah kosong.\n";
                // } else {
                    char setuju;
                    std::cout << "Setujui (approve) pendaftaran ini? (y/n): ";
                    setuju = read_line()[0];
                    service.processNextRegistration(setuju == 'y' || setuju == 'Y');
                // }
                pause();
                break;
            }
            case 2:
                handleUndoActionInteraktif(service);
                break;
            case 0:
                std::cout << "Kembali ke Dashboard Admin...\n";
                Sleep(1000);
                return;
            default:
                std::cout << "Pilihan tidak valid.\n";
                pause();
        }
    } while (true); // Loop ini akan berlanjut sampai pengguna memilih 0
}

static void runEventManagementSubMenu(AdminService& service) {
    int choice;
    do {
        clearScreen();
        showLogo();
        // Tampilkan daftar event setiap kali masuk sub-menu ini
        service.listAllEventsForAdmin(); 

        std::cout << '\n' << std::string(5, ' ') << std::string(45, '-') << " Pilihan Aksi Manajemen Event " << std::string(45, '-') << '\n';
        std::cout << R"(
            1. Tambah Event Baru             2. Hapus Event          3. Undo Aksi Terakhir            0. Kembali
)" << '\n' << '\n';
        std::cout << "Masukkan pilihan Anda: ";

        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -99; 
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1: handleAdminCreateEvent(service); break;
            case 2: handleAdminDeleteEvent(service); break;
            case 3: handleUndoActionInteraktif(service); break;
            case 0:
                std::cout << "Kembali ke Dashboard Admin...\n";
                Sleep(1000);
                return;
            default:
                std::cout << "Pilihan tidak valid.\n";
                pause();
        }
        // Setelah aksi CRUD, menu akan refresh dan menampilkan daftar event terbaru
    } while (true);
}


// Fungsi utama untuk loop menu admin (mirip runParticipantMenu)
int runAdminMenu(AdminService& service) {
    int choice;
    do {
        clearScreen();
        showLogo(); 
        showAdminMenu(); // Panggil menu utama admin yang baru
        
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -99; 
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1: 
                runVerificationSubMenu(service);
                break;
            case 2: 
                runEventManagementSubMenu(service);
                break;
            case 0:
                std::cout << "\nLogout dari sesi Admin...\n";
                Sleep(1500);
                return 0; // Kembali ke Menu Utama (main.cpp)
            default:
                std::cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                Sleep(1000);
        }
    } while (choice != 0); // Loop utama akan terus berjalan sampai user pilih 0
    return 0; 
}