#include "main_menu.h"

void showLogo() {
    std::cout << std::string(60, '=') << " Welcome to " << std::string(60, '=') << '\n';
    std::cout << R"(
                           ____                                                                            
                          /\  _`\                                                                           
                          \ \ \L\_\   _ __    ___    __  __  __    ____   _____      __       ___      __   
                           \ \ \L_L  /\`'__\ / __`\ /\ \/\ \/\ \  /',__\ /\ '__`\  /'__`\    /'___\  /'__`\ 
                            \ \ \/, \\ \ \/ /\ \L\ \\ \ \_/ \_/ \/\__, `\\ \ \L\ \/\ \L\.\_ /\ \__/ /\  __/ 
                             \ \____/ \ \_\ \ \____/ \ \___x___/'\/\____/ \ \ ,__/\ \__/.\_\\ \____\\ \____\
                              \/___/   \/_/  \/___/   \/__//__/   \/___/   \ \ \/  \/__/\/_/ \/____/ \/____/
                                                                            \ \_\                           
                                                                             \/_/                            

)";
    std::cout << std::string(41, '=') << " CLI Based App : Manajemen Workshop dan Pelatihan " << std::string(41, '=') << '\n';
    std::cout << std::string(50, '=') << " Developed by : Kelompok 7 - P4 " << std::string(50, '=') << "\n\n\n";
}

int showMainMenu() {
    while (true) {
        clearScreen();
        showLogo();

        std::cout << std::string(60, '-') << " Menu Utama " << std::string(60, '-') << '\n';
        std::cout << R"(
        1. Lihat dan Daftar Event                       2. Masuk sebagai Admin                        0. Keluar
)" << '\n';
        std::cout << "Masukkan pilihan anda [0 - 2] : ";

        int pilihan;
        std::cin >> pilihan;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input tidak valid. Masukkan angka.\n";
            Sleep(1000);
            continue;
        }

        switch (pilihan) {
            case 1:
                return 1;
            case 2: {
                std::string username, password;
                std::cout << "Masukkan username: ";
                std::cin >> username;
                std::cout << "Masukkan password: ";
                std::cin >> password;
                if (loginAdmin(username, password)) {
                    std::cout << "Selamat Datang Kembali " << username << '\n';
                    Sleep(2000);
                    return 2;
                } else {
                    std::cout << "Username atau password salah.\n";
                    Sleep(2000);
                }
                break;
            }
            case 0:
                return 0;
            default:
                std::cout << "Pilihan tidak valid.\n";
                Sleep(1000);
                break;
        }
    }
}

