#!/bin/bash

# Nama root folder
ROOT="growspace"

# Buat root directory dan file utama
mkdir -p "$ROOT"
touch "$ROOT/CMakeLists.txt"
touch "$ROOT/README.md"
touch "$ROOT/setup.sh"
touch "$ROOT/main.cpp"

# Folder data CSV
mkdir -p "$ROOT/data"
touch "$ROOT/data/events.csv"
touch "$ROOT/data/registrations.csv"
touch "$ROOT/data/users.csv"

# Header files
mkdir -p "$ROOT/include/menus"
mkdir -p "$ROOT/include/services"
mkdir -p "$ROOT/include/models"
mkdir -p "$ROOT/include/utils"

touch "$ROOT/include/menus/main_menu.h"
touch "$ROOT/include/menus/participant_menu.h"
touch "$ROOT/include/menus/admin_menu.h"

touch "$ROOT/include/services/participant_service.h"
touch "$ROOT/include/services/admin_service.h"
touch "$ROOT/include/services/auth_service.h"

touch "$ROOT/include/models.h"

touch "$ROOT/include/utils/csv_loader.h"
touch "$ROOT/include/utils/hashing.h"
touch "$ROOT/include/utils/validation.h"

# Source files
mkdir -p "$ROOT/src/menus"
mkdir -p "$ROOT/src/services"
mkdir -p "$ROOT/src/models"
mkdir -p "$ROOT/src/utils"

touch "$ROOT/src/menus/main_menu.cpp"
touch "$ROOT/src/menus/participant_menu.cpp"
touch "$ROOT/src/menus/admin_menu.cpp"

touch "$ROOT/src/services/participant_service.cpp"
touch "$ROOT/src/services/admin_service.cpp"
touch "$ROOT/src/services/auth_service.cpp"

touch "$ROOT/src/utils/csv_loader.cpp"
touch "$ROOT/src/utils/hashing.cpp"
touch "$ROOT/src/utils/validation.cpp"

# Tests
mkdir -p "$ROOT/tests"
touch "$ROOT/tests/test_participant.cpp"
touch "$ROOT/tests/test_admin.cpp"
touch "$ROOT/tests/test_csv_loader.cpp"

echo "Struktur direktori proyek '$ROOT' berhasil dibuat!"
