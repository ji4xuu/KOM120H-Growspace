Here’s a suggested **README.md** for your **Growspace** application:

---

## Growspace

Growspace is a command-line application for managing events and registrations. It supports two roles:

* **Participants** can browse events and register themselves.
* **Administrators** can manage users and view all registrations.

---

## Prerequisites

Before building and running Growspace, ensure you have:

* **CMake** ≥ 3.10
* **GCC** (or another C++17-capable compiler)
* **MinGW** (on Windows) or **Make** (on Linux/macOS)
* **Git** (optional, for source control)

---

## Getting Started

Clone the repository and prepare the build directory:

```bash
git clone https://github.com/yourusername/growspace.git
cd growspace
```

---

## Build

Use the provided `setup.sh` (Linux/macOS) or run CMake manually:

### Automated (Linux/macOS)

```bash
chmod +x setup.sh
./setup.sh
```

This will:

1. Create `build/`
2. Run `cmake -G "MinGW Makefiles" ..` (on Windows) or plain `cmake ..`
3. Invoke `make` to compile
4. Produce the `growspace` (or `growspace.exe`) binary in `build/`

### Manual

```bash
mkdir build
cd build

# On Windows with MinGW:
cmake -G "MinGW Makefiles" ..
cmake --build .

# On Linux/macOS:
cmake ..

make
```

---

## Usage

From the `build/` directory:

```bash
./growspace       # Linux/macOS
growspace.exe     # Windows (MinGW)
```

### Participant Flow

1. **Login** or **Register**
2. Select **Browse Events**
3. Choose an event and **Register**

### Admin Flow

1. **Login** as admin
2. Manage **Users** (view/add/remove)
3. View **All Registrations**

---

## Project Structure

```
Growspace/
├── CMakeLists.txt
├── README.md
├── setup.sh
│
├── main.cpp                        # Entry point aplikasi
│
├── data/                           # Folder semua data CSV
│   ├── events.csv
│   ├── registrations.csv
│   └── users.csv                   # (Admin only)
│
├── include/                        # Semua header file
│   ├── menus/                      # Tampilan antar muka CLI
│   │   ├── main_menu.h
│   │   ├── participant_menu.h
│   │   └── admin_menu.h
│   │
│   ├── services/                   # Logika layanan (bisnis)
│   │   ├── participant_service.h
│   │   ├── admin_service.h
│   │   └── auth_service.h
│   │
│   ├── models.h
│   │
│   └── utils/                      # Fungsi bantu (datetime, csv, validasi)
│       ├── csv_loader.h
│       ├── hashing.h
│       └── validation.h
│
├── src/                            # Implementasi dari header
│   ├── menus/
│   │   ├── main_menu.cpp
│   │   ├── participant_menu.cpp
│   │   └── admin_menu.cpp
│   │
│   ├── services/
│   │   ├── participant_service.cpp
│   │   ├── admin_service.cpp
│   │   └── auth_service.cpp
│   │
│   └── utils/
│       ├── csv_loader.cpp
│       ├── hashing.cpp
│       └── validation.cpp
│
└── tests/                          # Unit test
    ├── test_participant.cpp
    ├── test_admin.cpp
    └── test_csv_loader.cpp
```

---

## License

Nothing here
