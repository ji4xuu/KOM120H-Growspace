#include "admin_service.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <other.h>

AdminService::AdminService(
    std::vector<Event>& events, std::vector<Registration>& registrations, Queue<Registration>& verifQueue)
    : _events(events), _registrations(registrations), _verifQueue(verifQueue)
{
    buildAdminIndexes();
}

void AdminService::buildAdminIndexes() {
    _adminEventIndex.clear();
    _adminRegistrationIndex.clear();
    for (auto& event : _events) {
        _adminEventIndex[event.id] = &event;
    }
    for (auto& reg : _registrations) {
        _adminRegistrationIndex[reg.id] = &reg;
    }
}

void AdminService::viewVerificationQueue() const {
    if (_verifQueue.isEmpty()) {
        std::cout << "\nTidak ada pendaftaran yang perlu diverifikasi.\n";
        return;
    }
    std::cout << "\nAda " << _verifQueue.size() << " pendaftaran dalam antrean.\n";
    Registration frontReg = _verifQueue.front();
    std::cout << std::string(35, ' ') << std::string(61, '=') << '\n';
    std::cout << std::string(45, ' ') << " Pendaftar Berikutnya Untuk Diverifikasi \n";
    std::cout << std::string(35, ' ') << std::string(61, '=') << '\n';
    const int labelWidth = 18;
    std::cout << std::string(44, ' ') << std::left << std::setw(labelWidth) << "ID Registrasi" << ": " << frontReg.id << std::endl;
    std::cout << std::string(44, ' ') << std::left << std::setw(labelWidth) << "Nama Lengkap" << ": " << frontReg.full_name << std::endl;
    std::cout << std::string(44, ' ') << std::left << std::setw(labelWidth) << "Email" << ": " << frontReg.email << std::endl;
    std::cout << std::string(44, ' ') << std::left << std::setw(labelWidth) << "Event ID" << ": " << frontReg.event_id << std::endl;
    std::cout << std::string(44, ' ') << std::left << std::setw(labelWidth) << "NIK" << ": " << frontReg.nik << std::endl;
    std::cout << std::string(44, ' ') << std::left << std::setw(labelWidth) << "Akun Pembayaran" << ": " << frontReg.payment_account << std::endl;
    std::cout << std::string(35, ' ') << std::string(61, '=') << '\n';
}

bool AdminService::processNextRegistration(bool approve) {
    if (_verifQueue.isEmpty()) {
        std::cout << "\nAntrean verifikasi kosong.\n";
        return false;
    }
    Registration regToProcess = _verifQueue.front();
    Registration* originalReg = findRegistrationById(regToProcess.id);

    if (!originalReg) {
        std::cout << "Error: Data pendaftaran tidak konsisten. Pendaftaran dihapus dari antrean.\n";
        _verifQueue.dequeue();
        return false;
    }

    std::string oldState = std::to_string(static_cast<int>(originalReg->status)) + "," +
                           std::to_string(static_cast<int>(originalReg->payment_status));
    _actionStack.push(UndoRecord(ActionType::VERIFY_REGISTRATION, originalReg->id, oldState));

    originalReg->status = approve ? APPROVED : REJECTED;
    originalReg->payment_status = approve ? VERIFIED : FAILED;

    std::cout << "\n[BERHASIL] Registrasi ID " << originalReg->id << " telah diproses.\n";
    _verifQueue.dequeue();
    return true;
}

bool AdminService::createEvent(
    const std::string& title,
    const std::string& description,
    const std::string& start_date,
    const std::string& end_date,
    const std::string& registration_start,
    const std::string& registration_end,
    int quota,
    bool is_paid,
    const std::string& type
) {
    Event newEvent;
    newEvent.id = _events.empty() ? 1 : _events.back().id + 1;
    newEvent.title = title;
    newEvent.description = description;
    newEvent.start_date = start_date;
    newEvent.end_date = end_date;
    newEvent.registration_start = registration_start;
    newEvent.registration_end = registration_end;
    newEvent.quota = quota;
    newEvent.is_paid = is_paid;
    newEvent.type = type;
    newEvent.created_at = getCurrentDate();
    newEvent.status = AKTIF;

    _events.push_back(newEvent);
    buildAdminIndexes();
    _actionStack.push(UndoRecord(ActionType::CREATE_EVENT, newEvent.id, ""));

    std::cout << "\n[BERHASIL] Event baru '" << title << "' dengan ID " << newEvent.id << " telah dibuat.\n";
    return true;
}

bool AdminService::deleteEvent(int eventId) {
    for (const auto& reg : _registrations) {
        if (reg.event_id == eventId) {
            std::cout << "\n[GAGAL] Event ID " << eventId << " tidak bisa dihapus karena sudah memiliki peserta terdaftar.\n";
            return false;
        }
    }
    auto event_it = std::find_if(_events.begin(), _events.end(),
                                 [eventId](const Event& ev) { return ev.id == eventId; });
    if (event_it == _events.end()) {
        std::cout << "\n[GAGAL] Event ID " << eventId << " tidak ditemukan.\n";
        return false;
    }
    std::string eventData = serializeEvent(*event_it);
    _actionStack.push(UndoRecord(ActionType::DELETE_EVENT, eventId, eventData));
    _events.erase(event_it);
    buildAdminIndexes();
    std::cout << "\n[BERHASIL] Event ID " << eventId << " telah dihapus.\n";
    return true;
}

bool AdminService::undoLastAction() {
    if (_actionStack.isEmpty()) {
        std::cout << "\nTidak ada aksi yang bisa di-undo.\n";
        return false;
    }
    UndoRecord lastAction = _actionStack.top();
    _actionStack.pop();

    switch (lastAction.action) {
        case ActionType::VERIFY_REGISTRATION: {
            Registration* reg = findRegistrationById(lastAction.target_id);
            if (reg) {
                std::stringstream ss(lastAction.pre_state);
                std::string reg_status_str, pay_status_str;
                std::getline(ss, reg_status_str, ',');
                std::getline(ss, pay_status_str, ',');
                reg->status = static_cast<RegistStatus>(std::stoi(reg_status_str));
                reg->payment_status = static_cast<PaymentStatus>(std::stoi(pay_status_str));
                std::cout << "\n[UNDO] Verifikasi pada registrasi ID " << lastAction.target_id << " dibatalkan.\n";
                if (reg->status == PENDING) {
                    _verifQueue.enqueueFront(*reg);
                    std::cout << "Registrasi ID " << reg->id << " dimasukkan kembali ke DEPAN antrean verifikasi.\n";
                }
            } else {
                std::cout << "\n[GAGAL UNDO] Registrasi ID " << lastAction.target_id << " tidak ditemukan.\n";
            }
            break;
        }
        case ActionType::CREATE_EVENT: {
            auto it = std::find_if(_events.begin(), _events.end(),
                                   [id = lastAction.target_id](const Event& ev) { return ev.id == id; });
            if (it != _events.end()) {
                _events.erase(it);
                buildAdminIndexes();
                std::cout << "\n[UNDO] Pembuatan event ID " << lastAction.target_id << " dibatalkan.\n";
            }
            break;
        }
        case ActionType::DELETE_EVENT: {
            Event restoredEvent = deserializeEvent(lastAction.pre_state);
            _events.push_back(restoredEvent);
            buildAdminIndexes();
            std::cout << "\n[UNDO] Penghapusan event ID " << lastAction.target_id << " dibatalkan.\n";
            break;
        }
        default:
            std::cout << "\n[GAGAL] Tipe aksi undo tidak dikenali.\n";
            break;
    }
    return true;
}

void AdminService::listAllEventsForAdmin() const {
    std::cout << std::string(20, ' ') << std::string(29, '-') << " Daftar Semua Event (Admin View) " << std::string(29, '-') << '\n';
    if (_events.empty()) {
        std::cout << "Tidak ada event yang tersedia.\n";
        return;
    }
    constexpr int TABLE_WIDTH = 132;
    std::cout << std::string(TABLE_WIDTH, '-') << '\n';
    std::cout << std::left
              << std::setw(4) << "ID"
              << std::setw(40) << "Judul"
              << std::setw(26) << "Pendaftaran"
              << std::setw(26) << "Pelaksanaan"
              << std::setw(10) << "Berbayar"
              << std::setw(15) << "Tipe"
              << std::setw(11) << "Kuota" << '\n';
    std::cout << std::string(TABLE_WIDTH, '-') << '\n';
    for (const auto& ev : _events) {
        std::string reg_period = ev.registration_start + " - " + ev.registration_end;
        std::string exec_period = ev.start_date + " - " + ev.end_date;
        std::cout << std::setw(4) << ev.id
                  << std::setw(40) << ev.title
                  << std::setw(26) << reg_period
                  << std::setw(26) << exec_period
                  << std::setw(10) << (ev.is_paid ? "Ya" : "Tidak")
                  << std::setw(15) << ev.type
                  << std::setw(11) << ev.quota << '\n';
    }
}

std::string AdminService::peekLastActionDescription() const {
    if (_actionStack.isEmpty()) {
        return "Tidak ada aksi untuk di-undo.";
    }
    UndoRecord lastAction = _actionStack.top();
    std::string desc = "Aksi terakhir yang akan dibatalkan: ";

    switch (lastAction.action) {
        case ActionType::VERIFY_REGISTRATION: {
            std::stringstream ss(lastAction.pre_state);
            std::string reg_status_num_str, pay_status_num_str;
            std::string reg_status_text, pay_status_text;
            desc += "VERIFIKASI REGISTRASI\n";
            desc += "  Detail: Registrasi ID " + std::to_string(lastAction.target_id);
            if (std::getline(ss, reg_status_num_str, ',') && std::getline(ss, pay_status_num_str, ',')) {
                try {
                    RegistStatus oldRegStatus = static_cast<RegistStatus>(std::stoi(reg_status_num_str));
                    PaymentStatus oldPayStatus = static_cast<PaymentStatus>(std::stoi(pay_status_num_str));
                    switch (oldRegStatus) {
                        case PENDING:   reg_status_text = "PENDING";   break;
                        case APPROVED:  reg_status_text = "APPROVED";  break;
                        case REJECTED:  reg_status_text = "REJECTED";  break;
                        default:        reg_status_text = "CANCELLED"; break;
                    }
                    switch (oldPayStatus) {
                        case UNVERIFIED: pay_status_text = "UNVERIFIED"; break;
                        case VERIFIED:   pay_status_text = "VERIFIED";   break;
                        default:         pay_status_text = "FAILED";     break;
                    }
                    desc += " akan dikembalikan ke status:\n";
                    desc += "    Status Pendaftaran: " + reg_status_text + "\n";
                    desc += "    Status Pembayaran : " + pay_status_text;
                } catch (...) {
                    desc += " (gagal membaca status lama dari: " + lastAction.pre_state + ")";
                }
            } else {
                desc += " (format status lama tidak valid: " + lastAction.pre_state + ")";
            }
            break;
        }
        case ActionType::CREATE_EVENT:
            desc += "BUAT EVENT BARU\n";
            desc += "  Detail: Event ID " + std::to_string(lastAction.target_id) + " akan dihapus";
            break;
        case ActionType::DELETE_EVENT:
            desc += "HAPUS EVENT\n";
            desc += "  Detail: Event ID " + std::to_string(lastAction.target_id) +
                    " akan dikembalikan (data: " + lastAction.pre_state + ")";
            break;
        default:
            desc = "Aksi terakhir tidak dikenali.";
            break;
    }
    return desc;
}

Event* AdminService::findEventById(int eventId) {
    auto it = _adminEventIndex.find(eventId);
    return (it != _adminEventIndex.end()) ? it->second : nullptr;
}

Registration* AdminService::findRegistrationById(int registrationId) {
    auto it = _adminRegistrationIndex.find(registrationId);
    return (it != _adminRegistrationIndex.end()) ? it->second : nullptr;
}

std::string AdminService::serializeEvent(const Event& event) const {
    std::ostringstream oss;
    oss << event.id << ";"
        << event.title << ";"
        << event.description << ";"
        << event.start_date << ";"
        << event.end_date << ";"
        << event.registration_start << ";"
        << event.registration_end << ";"
        << event.quota << ";"
        << (event.is_paid ? "1" : "0") << ";"
        << event.type << ";"
        << event.created_at << ";"
        << static_cast<int>(event.status);
    return oss.str();
}

Event AdminService::deserializeEvent(const std::string& eventString) const {
    std::istringstream iss(eventString);
    std::string part;
    Event event;
    std::getline(iss, part, ';'); event.id = std::stoi(part);
    std::getline(iss, part, ';'); event.title = part;
    std::getline(iss, part, ';'); event.description = part;
    std::getline(iss, part, ';'); event.start_date = part;
    std::getline(iss, part, ';'); event.end_date = part;
    std::getline(iss, part, ';'); event.registration_start = part;
    std::getline(iss, part, ';'); event.registration_end = part;
    std::getline(iss, part, ';'); event.quota = std::stoi(part);
    std::getline(iss, part, ';'); event.is_paid = (part == "1");
    std::getline(iss, part, ';'); event.type = part;
    std::getline(iss, part, ';'); event.created_at = part;
    std::getline(iss, part, ';'); event.status = static_cast<EventStatus>(std::stoi(part));
    return event;
}