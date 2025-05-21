#include <participant_service.h>


ParticipantService::ParticipantService(std::vector<Event>& events, std::vector<Registration>& registrations, Queue<Registration>& verifQueue) : 
_events(events), _registrations(registrations), _verifQueue(verifQueue)
{}

Event ParticipantService::findEventId(int eventId) const {
    for (const auto& ev : _events) {
        if (ev.id == eventId)
            return ev;
    }
    // default-constructed Event sebagai “not found”
    Event none{};
    none.id = -1;
    return none;
}

int ParticipantService::hitungSisaKuota(const Event& ev) const{
    int terpakai = 0;
    for (const auto& reg : _registrations) {
        if (reg.event_id == ev.id &&
            (reg.status == APPROVED || reg.status == PENDING)) {
            terpakai++;
        }
    }
    return ev.quota - terpakai;
}


void ParticipantService::showEvents(const std::vector<Event> &events, const std::string &keyword, std::string &sortBy, bool &isAscending) {
    std::cout << "Menampilkan semua event, difilter berdasarkan <" << keyword << "> diurutkan berdasarkan <" << sortBy
              << "> secara " << (isAscending ? "menaik" : "menurun") << ".\n";

    // Header tabel
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

    // Isi tabel
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
Registration* ParticipantService::getMyRegistrationStatus(int registrationId) {
    for (auto& regs : _registrations) {
        if (regs.id == registrationId)
            return &regs; // return pointer ke elemen asli
    }
    return nullptr; // tidak ditemukan
}

std::vector<Event> ParticipantService::listEvents(const std::string& keyword, const std::string& sortBy, bool ascending) const{
    std::vector<Event> result;

    for (const auto& ev : _events){
        if (keyword == "-" || ev.title.find(keyword) != std::string::npos || ev.description.find(keyword) != std::string::npos || 
        ev.start_date.find(keyword) != std::string::npos || ev.end_date.find(keyword) != std::string::npos){
            result.push_back(ev);
        }
    }
    auto comparator = [&](const Event& a, const Event& b) {
        if (sortBy == "id" || sortBy == "-") {return ascending ? a.id < b.id : a.id > b.id;}
        else if (sortBy == "title") {return ascending ? a.title < b.title : a.title > b.title;}
        else if (sortBy == "tanggal_mulai") {return ascending ? a.start_date < b.start_date : a.start_date > b.start_date;}
        else if (sortBy == "tanggal_selesai") {return ascending ? a.end_date < b.end_date : a.end_date > b.end_date;}
        else if (sortBy == "tanggal_mulai_daftar") {return ascending ? a.registration_start < b.registration_start : a.registration_start > b.registration_start;}
        else if (sortBy == "tanggal_selesai_daftar") {return ascending ? a.registration_end < b.registration_end : a.registration_end > b.registration_end;}
        else if (sortBy == "sisa_kuota") {return ascending ? hitungSisaKuota(a) < hitungSisaKuota(b) : hitungSisaKuota(a) > hitungSisaKuota(b);}
        else
            return false; // Default: tidak mengubah urutan
    };

    std::sort(result.begin(), result.end(), comparator);
    return result;
}

bool ParticipantService::registerToEvent(int eventId){
    Event ev = findEventId(eventId);
    if(ev.id == -1){
        std::cout << "Event tidak ditemukan." << '\n';
        return true;
    }
    int sisa_kuota = hitungSisaKuota(ev);
    if(sisa_kuota <= 0){
        std::cout << "Kuota penuh." << '\n';
        return true;
    }
    std::string full_name, ttl, nik, email, password, payment_acc;

    Registration r;

    do {
        clearScreen();
        std::cout << "Masukkan informasi berikut untuk menyelesaikan pendaftaran." << '\n';
        std::cout << "Nama Lengkap                : ";
        std::getline(std::cin, full_name);
        if (full_name.empty()){
            std::cout << "Nama tidak boleh kosong.\n";
            Sleep(1000);
            continue;
        }
        std::cout << "Tanggal Lahir (YYYY-MM-DD)  : ";
        std::getline(std::cin, ttl);
        if (ttl.empty()){
            std::cout << "Tanggal lahir tidak boleh kosong. (contoh: Jakarta, 2000-01-31).\n";
            Sleep(1000);
            continue;
        }
        std::cout << "NIK (16 digit)              : ";
        std::getline(std::cin, nik);
        if (nik.size() != 16 || !std::all_of(nik.begin(), nik.end(), ::isdigit)){
            std::cout << "NIK harus 16 digit angka.\n";
            Sleep(1000);
            continue;
        }
        std::cout << "Email                       : ";
        std::getline(std::cin, email);
        auto at = email.find('@');
        auto dot = email.rfind('.');
        if (at == std::string::npos || dot == std::string::npos || dot < at){
            std::cout << "Format email tidak valid.\n";
            Sleep(1000);
            continue;
        }
        std::cout << "Password (>=8 char)         : ";
        std::getline(std::cin, password);
        if (password.length() < 8){
            std::cout << "Password minimal 8 karakter.\n";
            Sleep(1000);
            continue;
        }
        if (ev.is_paid) {
            std::cout << "No. Rekening/VA             : ";
            std::getline(std::cin, payment_acc);
            if (payment_acc.empty()){
                std::cout << "Harap masukkan nomor pembayaran.\n";
                Sleep(1000);
                continue;
            }
            r.payment_status = UNVERIFIED;
        } else {
            payment_acc     = "-";
            r.payment_status  = VERIFIED;
        }
        r.payment_account = payment_acc;
    } while (full_name.empty() || ttl.empty() || nik.size() != 16 || !std::all_of(nik.begin(), nik.end(), ::isdigit) ||
    email.find('@') == std::string::npos || email.rfind('.') == std::string::npos || email.rfind('.') < email.find('@')||
    password.length() < 8 || payment_acc.empty());


    r.id            = static_cast<int>(_registrations.size()) + 1;
    r.event_id      = eventId;
    r.status        = ev.is_paid
                      ? PENDING
                      : APPROVED;
    r.created_at = getCurrentDate();
    r.full_name       = full_name;
    r.ttl             = ttl;
    r.nik             = nik;
    r.email           = email;
    r.password_hash   = password;

    _registrations.push_back(r);
    if (ev.is_paid) {
        _verifQueue.enqueue(r);
    }
    Registration* reg = getMyRegistrationStatus(r.id);
    clearScreen();
    std::cout << "Pendaftaran berhasil.\n";
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

    constexpr int LBL_W = 18;  
    std::cout << "=== Detail Status Registrasi ===\n\n";
    std::cout << std::left << std::setw(LBL_W) << "ID Registrasi"   << ": " << reg->id            << "\n";
    std::cout << std::left << std::setw(LBL_W) << "Event ID"         << ": " << reg->event_id     << "\n";
    std::cout << std::left << std::setw(LBL_W) << "Nama Lengkap"    << ": " << reg->full_name    << "\n";
    std::cout << std::left << std::setw(LBL_W) << "Tanggal Lahir"   << ": " << reg->ttl           << "\n";
    std::cout << std::left << std::setw(LBL_W) << "NIK"             << ": " << reg->nik           << "\n";
    std::cout << std::left << std::setw(LBL_W) << "Email"           << ": " << reg->email         << "\n";
    std::cout << std::left << std::setw(LBL_W) << "Status Pendaftaran" << ": " << status_str      << "\n";
    std::cout << std::left << std::setw(LBL_W) << "Status Pembayaran"  << ": " << payment_str << "\n";
    std::cout << std::left << std::setw(LBL_W) << "Dibuat pada"      << ": " << reg->created_at    << "\n\n";
    std::cout << "Simpan baik-baik data registrasi anda.\n";
    return false;
}

void ParticipantService::getEventDetail(const Event &ev) const {
    int sisa = hitungSisaKuota(ev);

    // Ganti '|' jadi '\n' dan split ke vector
    std::string desc = ev.description;
    std::replace(desc.begin(), desc.end(), '|', '\n');
    std::vector<std::string> lines;
    std::stringstream ss(desc);
    std::string line;
    while (std::getline(ss, line, '\n')) {
        lines.push_back(line);
    }

    constexpr int LABEL_WIDTH = 14; 

    std::cout << std::left << std::setw(LABEL_WIDTH) << "Judul" 
              << ": " << ev.title << "\n";

    // Baris pertama deskripsi
    if (!lines.empty()) {
        std::cout << std::left << std::setw(LABEL_WIDTH) << "Deskripsi"
                  << ": " << lines[0] << "\n";
        // Baris–baris selanjutnya, indent sejajar setelah ": "
        std::string indent(LABEL_WIDTH + 2, ' ');
        for (size_t i = 1; i < lines.size(); ++i) {
            std::cout << indent << lines[i] << "\n";
        }
    } else {
        // Deskripsi kosong
        std::cout << std::left << std::setw(LABEL_WIDTH) << "Deskripsi"
                  << ": \n";
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
