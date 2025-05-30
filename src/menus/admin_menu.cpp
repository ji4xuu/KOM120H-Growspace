#include "admin_menu.h"
#include "admin_service.h"
#include "main_menu.h"
#include "utils/other.h"
#include <iostream>
#include <string>
#include <limits>
#include <iomanip>

// Tampilkan menu utama admin
static void showAdminMenu() {
    std::cout << std::string(6, ' ') << std::string(51, '-') << " Dashboard Admin " << std::string(51, '-') << '\n';
    std::cout << R"(
            1. Verifikasi Pendaftaran                   2. Manajemen Event                    0. Logout
)" << '\n' << '\n';
    std::cout << "Masukkan pilihan anda [0 - 2] : ";
}

// Handler tambah event baru
static void handleAdminCreateEvent(AdminService &service) {
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
        if (!isValidDateFormat(end_date)) {
            std::cout << "Format Tanggal Selesai salah (YYYY-MM-DD). Masukkan lagi: ";
            continue;
        }
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
        if (!isValidDateFormat(registration_start)) {
            std::cout << "Format Tgl Mulai Pendaftaran salah (YYYY-MM-DD). Masukkan lagi: ";
            continue;
        }
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
        if (!isValidDateFormat(registration_end)) {
            std::cout << "Format Tgl Selesai Pendaftaran salah (YYYY-MM-DD). Masukkan lagi: ";
            continue;
        }
        if (registration_end < registration_start) {
            std::cout << "Tgl Selesai Pendaftaran tidak boleh sebelum Tgl Mulai Pendaftaran (" << registration_start << "). Masukkan lagi: ";
            continue;
        }
        if (registration_end > end_date) {
            std::cout << "Tgl Selesai Pendaftaran tidak boleh setelah Tanggal Selesai Event (" << end_date << "). Masukkan lagi: ";
            continue;
        }
        if (registration_end > start_date) {
            std::cout << "Peringatan: Tgl Selesai Pendaftaran (" << registration_end << ") melewati Tanggal Mulai Event (" << start_date << ").\n";
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

    std::cout << "Tipe Event (contoh: Workshop, Seminar)     : ";
    type_str = read_line();

    service.createEvent(title, description, start_date, end_date, registration_start, registration_end, quota_int, is_paid_bool, type_str);
    pause();
}

// Handler hapus event
static void handleAdminDeleteEvent(AdminService &service) {
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

    Event *eventToDelete = service.findEventById(eventId);

    if (eventToDelete == nullptr) {
        std::cout << "\nEvent dengan ID " << eventId << " tidak ditemukan.\n";
    } else {
        std::cout << std::string(35, ' ') << std::string(61, '=') << '\n';
        std::cout << std::string(50, ' ') << " Detail Event yang Akan Dihapus \n";
        std::cout << std::string(35, ' ') << std::string(61, '=') << '\n';

        const int labelWidth = 22;
        std::cout << std::string(41, ' ') << std::left << std::setw(labelWidth) << "ID Event" << ": " << eventToDelete->id << std::endl;
        std::cout << std::string(41, ' ') << std::left << std::setw(labelWidth) << "Judul" << ": " << eventToDelete->title << std::endl;
        std::cout << std::string(41, ' ') << std::left << std::setw(labelWidth) << "Tanggal Pendaftaran" << ": " << eventToDelete->start_date << " - " << eventToDelete->end_date << std::endl;
        std::cout << std::string(41, ' ') << std::left << std::setw(labelWidth) << "Tanggal Pelaksanaan" << ": " << eventToDelete->registration_start << " - " << eventToDelete->registration_end << std::endl;
        std::cout << std::string(41, ' ') << std::left << std::setw(labelWidth) << "Tipe" << ": " << eventToDelete->type << std::endl;
        std::cout << std::string(41, ' ') << std::left << std::setw(labelWidth) << "Kuota" << ": " << eventToDelete->quota << std::endl;
        std::cout << std::string(35, ' ') << std::string(61, '=') << '\n';

        char confirm;
        std::cout << "\nApakah Anda yakin ingin menghapus event '" << eventToDelete->title << "' (ID: " << eventId << ")? (y/n): ";
        confirm = read_line()[0];

        if (confirm == 'y' || confirm == 'Y') {
            service.deleteEvent(eventId);
        } else {
            std::cout << "\nPenghapusan event dibatalkan.\n";
        }
    }
    pause();
}

// Handler undo aksi terakhir
static void handleUndoActionInteraktif(AdminService &service) {
    clearScreen();
    showLogo();
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
        if (!service.undoLastAction()) {
            std::cout << "Gagal melakukan undo.\n";
        }
    } else {
        std::cout << "Undo dibatalkan.\n";
    }
    pause();
}

// Handler proses verifikasi pendaftaran
static void handleAdminProcessRegistration(AdminService &service) {
    clearScreen();
    showLogo();
    std::cout << std::string(6, ' ') << std::string(45, '-') << " Proses Verifikasi Pendaftaran " << std::string(45, '-') << '\n';

    service.viewVerificationQueue();

    char pilihan;
    std::cout << "\nProses pendaftar berikutnya? (y/n): ";
    pilihan = read_line()[0];

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

// Sub-menu verifikasi pendaftaran
static void runVerificationSubMenu(AdminService &service) {
    int choice;
    do {
        clearScreen();
        showLogo();
        std::cout << std::string(6, ' ') << std::string(45, '-') << " Proses Verifikasi Pendaftaran " << std::string(45, '-') << '\n';
        service.viewVerificationQueue();

        std::cout << std::string(8, ' ') << std::string(45, '-') << " Pilihan Aksi Verifikasi " << std::string(45, '-') << '\n';
        std::cout << R"(
            1. Proses Pendaftar Berikutnya              2. Undo Aksi Terakhir                  0. Kembali
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
            case 1: {
                char setuju;
                std::cout << "Setujui (approve) pendaftaran ini? (y/n): ";
                setuju = read_line()[0];
                service.processNextRegistration(setuju == 'y' || setuju == 'Y');
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
    } while (true);
}

// Sub-menu manajemen event
static void runEventManagementSubMenu(AdminService &service) {
    int choice;
    do {
        clearScreen();
        showLogo();
        service.listAllEventsForAdmin();

        std::cout << '\n'
                  << std::string(5, ' ') << std::string(45, '-') << " Pilihan Aksi Manajemen Event " << std::string(45, '-') << '\n';
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
            case 1:
                handleAdminCreateEvent(service);
                break;
            case 2:
                handleAdminDeleteEvent(service);
                break;
            case 3:
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
    } while (true);
}

// Fungsi utama menu admin
int runAdminMenu(AdminService &service) {
    int choice;
    do {
        clearScreen();
        showLogo();
        showAdminMenu();

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
                return 0;
            default:
                std::cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                Sleep(1000);
        }
    } while (choice != 0);
    return 0;
}