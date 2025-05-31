#include <participant_service.h>
#include <algorithm>
#include <sstream>
#include <iomanip>

// Konstruktor: inisialisasi dan bangun indeks
ParticipantService::ParticipantService(std::vector<Event>& events, std::vector<Registration>& registrations, Queue<Registration>& verifQueue)
    : _events(events), _registrations(registrations), _verifQueue(verifQueue) {
    buildIndexes();
}

// Cari event berdasarkan ID
Event* ParticipantService::findEventById(int eventId) const {
    auto it = _eventIndex.find(eventId);
    return (it != _eventIndex.end()) ? it->second : nullptr;
}

// Hitung sisa kuota event
int ParticipantService::hitungSisaKuota(const Event& ev) const {
    int terpakai = 0;
    for (const auto& reg : _registrations) {
        if (reg.event_id == ev.id && (reg.status == APPROVED || reg.status == PENDING)) {
            terpakai++;
        }
    }
    return ev.quota - terpakai;
}

// Tampilkan daftar event dengan filter dan sorting
void ParticipantService::showEvents(const std::vector<Event>& events, const std::string& keyword, std::string& sortBy, bool& isAscending) {
    std::cout << "Menampilkan semua event, difilter berdasarkan <" << keyword << "> diurutkan berdasarkan <" << sortBy
              << "> secara " << (isAscending ? "menaik" : "menurun") << ".\n";

    std::cout << std::string(132, '-') << '\n';
    std::cout << std::left
              << std::setw(4) << "ID"
              << std::setw(40) << "Judul"
              << std::setw(26) << "Pendaftaran"
              << std::setw(26) << "Pelaksanaan"
              << std::setw(10) << "Berbayar"
              << std::setw(15) << "Tipe"
              << std::setw(11) << "Sisa" << '\n';
    std::cout << std::string(132, '-') << '\n';

    for (const auto& ev : events) {
        std::string reg_period = ev.registration_start + " - " + ev.registration_end;
        std::string exec_period = ev.start_date + " - " + ev.end_date;
        int sisa_kuota = hitungSisaKuota(ev);
        std::string kuotaInfo = std::to_string(sisa_kuota) + "/" + std::to_string(ev.quota);

        std::cout << std::setw(4) << ev.id
                  << std::setw(40) << ev.title
                  << std::setw(26) << reg_period
                  << std::setw(26) << exec_period
                  << std::setw(10) << (ev.is_paid ? "Ya" : "Tidak")
                  << std::setw(15) << ev.type
                  << std::setw(11) << kuotaInfo << '\n';
    }
}

// Cari registrasi berdasarkan ID
Registration* ParticipantService::getMyRegistrationStatus(int registrationId) {
    auto it = _registrationIndex.find(registrationId);
    return (it != _registrationIndex.end()) ? it->second : nullptr;
}

// List event dengan filter dan sorting
std::vector<Event> ParticipantService::listEvents(const std::string& keyword, const std::string& sortBy, bool ascending) const {
    std::vector<Event> result;
    for (const auto& ev : _events) {
        if ((ev.status == AKTIF || ev.status == SELESAI) &&
            (keyword == "-" || ev.title.find(keyword) != std::string::npos ||
             ev.description.find(keyword) != std::string::npos ||
             ev.start_date.find(keyword) != std::string::npos ||
             ev.end_date.find(keyword) != std::string::npos)) {
            result.push_back(ev);
        }
    }
    auto comparator = [&](const Event& a, const Event& b) {
        if (sortBy == "id" || sortBy == "-") return ascending ? a.id < b.id : a.id > b.id;
        if (sortBy == "title") return ascending ? a.title < b.title : a.title > b.title;
        if (sortBy == "tanggal_mulai") return ascending ? a.start_date < b.start_date : a.start_date > b.start_date;
        if (sortBy == "tanggal_selesai") return ascending ? a.end_date < b.end_date : a.end_date > b.end_date;
        if (sortBy == "tanggal_mulai_daftar") return ascending ? a.registration_start < b.registration_start : a.registration_start > b.registration_start;
        if (sortBy == "tanggal_selesai_daftar") return ascending ? a.registration_end < b.registration_end : a.registration_end > b.registration_end;
        if (sortBy == "sisa_kuota") return ascending ? hitungSisaKuota(a) < hitungSisaKuota(b) : hitungSisaKuota(a) > hitungSisaKuota(b);
        return false;
    };
    std::sort(result.begin(), result.end(), comparator);
    return result;
}

// Proses pendaftaran peserta ke event
bool ParticipantService::registerToEvent(int eventId) {
    Event* ev = findEventById(eventId);
    if (!ev) {
        std::cout << "Event tidak ditemukan.\n";
        return true;
    }
    if (ev->status != AKTIF) {
        std::cout << "Event ini tidak aktif dan tidak bisa didaftar.\n";
        return true;
    }
    int sisa_kuota = hitungSisaKuota(*ev);
    if (sisa_kuota <= 0) {
        std::cout << "Kuota penuh.\n";
        return true;
    }

    Registration r;
    std::string full_name, ttl, nik, email, password, payment_acc;

    // Input data peserta
    while (true) {
        clearScreen();
        std::cout << "Masukkan informasi berikut untuk menyelesaikan pendaftaran.\n";
        std::cout << "Nama Lengkap                          : ";
        std::getline(std::cin, full_name);
        if (full_name.empty()) { std::cout << "Nama tidak boleh kosong.\n"; Sleep(1000); continue; }

        std::cout << "Tanggal Lahir (YYYY-MM-DD)            : ";
        std::getline(std::cin, ttl);
        if (ttl.empty() || !isValidDateFormat(ttl)) { std::cout << "Tanggal lahir tidak valid.\n"; Sleep(1000); continue; }

        std::cout << "NIK (16 digit)                        : ";
        std::getline(std::cin, nik);
        if (nik.size() != 16 || !std::all_of(nik.begin(), nik.end(), ::isdigit)) { std::cout << "NIK harus 16 digit angka.\n"; Sleep(1000); continue; }

        std::cout << "Email                                 : ";
        std::getline(std::cin, email);
        auto at = email.find('@'), dot = email.rfind('.');
        if (at == std::string::npos || dot == std::string::npos || dot < at) { std::cout << "Format email tidak valid.\n"; Sleep(1000); continue; }

        std::cout << "Password (>=8 char)                   : ";
        std::getline(std::cin, password);
        if (password.length() < 8) { std::cout << "Password minimal 8 karakter.\n"; Sleep(1000); continue; }

        if (ev->is_paid) {
        std::cout << "No. Rekening (Contoh : BCA - 1234)    : ";
            std::getline(std::cin, payment_acc);
            if (payment_acc.empty()) { std::cout << "Harap masukkan nomor pembayaran.\n"; Sleep(1000); continue; }
            r.payment_status = UNVERIFIED;
        } else {
            payment_acc = "-";
            r.payment_status = VERIFIED;
        }
        r.payment_account = payment_acc;
        break;
    }

    // Set data registrasi
    r.id            = static_cast<int>(_registrations.size()) + 1;
    r.event_id      = eventId;
    r.status        = ev->is_paid ? PENDING : APPROVED;
    r.created_at    = getCurrentDate();
    r.full_name     = full_name;
    r.ttl           = ttl;
    r.nik           = nik;
    r.email         = email;
    r.password_hash = hashPassword(password);

    _registrations.push_back(r);
    buildIndexes();
    if (ev->is_paid) {
        _verifQueue.enqueue(r);
    }

    Registration* reg = getMyRegistrationStatus(r.id);
    pause();
    clearScreen();
    std::cout << "Pendaftaran berhasil.\n";

    // Tampilkan status registrasi
    std::string status_str, payment_str;
    switch (reg->status) {
        case PENDING:   status_str = "Pending";   break;
        case APPROVED:  status_str = "Approved";  break;
        case REJECTED:  status_str = "Rejected";  break;
        case CANCELLED: status_str = "Cancelled"; break;
    }
    switch (reg->payment_status) {
        case VERIFIED:   payment_str = "Verified";   break;
        case UNVERIFIED: payment_str = "Unverified"; break;
        case FAILED:     payment_str = "Failed";     break;
    }

    constexpr int LBL_W = 18;
    std::cout << "=== Detail Status Registrasi ===\n\n";
    std::cout << std::left << std::setw(LBL_W) << "ID Registrasi"      << ": " << reg->id          << "\n";
    std::cout << std::left << std::setw(LBL_W) << "Event ID"           << ": " << reg->event_id    << "\n";
    std::cout << std::left << std::setw(LBL_W) << "Nama Lengkap"       << ": " << reg->full_name   << "\n";
    std::cout << std::left << std::setw(LBL_W) << "Tanggal Lahir"      << ": " << reg->ttl         << "\n";
    std::cout << std::left << std::setw(LBL_W) << "NIK"                << ": " << reg->nik         << "\n";
    std::cout << std::left << std::setw(LBL_W) << "Email"              << ": " << reg->email       << "\n";
    std::cout << std::left << std::setw(LBL_W) << "Status Pendaftaran" << ": " << status_str       << "\n";
    std::cout << std::left << std::setw(LBL_W) << "Status Pembayaran"  << ": " << payment_str      << "\n";
    std::cout << std::left << std::setw(LBL_W) << "Dibuat pada"        << ": " << reg->created_at  << "\n\n";
    std::cout << "Simpan baik-baik data registrasi anda.\n";
    return true;
}

// Tampilkan detail event
void ParticipantService::getEventDetail(const Event& ev) const {
    int sisa = hitungSisaKuota(ev);

    // Ubah '|' menjadi baris baru
    std::string desc = ev.description;
    std::replace(desc.begin(), desc.end(), '|', '\n');
    std::vector<std::string> lines;
    std::stringstream ss(desc);
    std::string line;
    while (std::getline(ss, line, '\n')) {
        lines.push_back(line);
    }

    constexpr int LABEL_WIDTH = 14;
    std::cout << std::left << std::setw(LABEL_WIDTH) << "Judul" << ": " << ev.title << "\n";
    if (!lines.empty()) {
        std::cout << std::left << std::setw(LABEL_WIDTH) << "Deskripsi" << ": " << lines[0] << "\n";
        std::string indent(LABEL_WIDTH + 2, ' ');
        for (size_t i = 1; i < lines.size(); ++i) {
            std::cout << indent << lines[i] << "\n";
        }
    } else {
        std::cout << std::left << std::setw(LABEL_WIDTH) << "Deskripsi" << ": \n";
    }
    std::cout << std::left << std::setw(LABEL_WIDTH) << "Pendaftaran"
              << ": " << ev.registration_start << " s/d " << ev.registration_end << "\n";
    std::cout << std::left << std::setw(LABEL_WIDTH) << "Pelaksanaan"
              << ": " << ev.start_date << " s/d " << ev.end_date << "\n";
    std::cout << std::left << std::setw(LABEL_WIDTH) << "Berbayar"
              << ": " << (ev.is_paid ? "Ya" : "Tidak") << "\n";
    std::cout << std::left << std::setw(LABEL_WIDTH) << "Tipe"
              << ": " << ev.type << "\n";
    std::cout << std::left << std::setw(LABEL_WIDTH) << "Kuota"
              << ": " << sisa << "/" << ev.quota << "\n\n";
}

// Bangun ulang indeks event dan registrasi
void ParticipantService::buildIndexes() {
    _eventIndex.clear();
    _registrationIndex.clear();
    for (auto& event : _events) {
        _eventIndex[event.id] = &event;
    }
    for (auto& reg : _registrations) {
        _registrationIndex[reg.id] = &reg;
    }
}