cmake_minimum_required(VERSION 3.10)
project(Growspace VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Direktori header
include_directories(include include/menus include/services include/utils)

# Sumber utama
file(GLOB_RECURSE SOURCES
    src/*.cpp
    main.cpp
)

add_executable(growspace ${SOURCES})

# Unit test (opsional, aktifkan jika diperlukan)
# enable_testing()
# add_executable(test_participant tests/test_participant.cpp src/services/participant_service.cpp src/utils/csv_loader.cpp)
# target_include_directories(test_participant PRIVATE include include/services include/utils)
# add_executable(test_admin tests/test_admin.cpp src/services/admin_service.cpp src/utils/csv_loader.cpp)
# target_include_directories(test_admin PRIVATE include include/services include/utils)
# add_executable(test_csv_loader tests/test_csv_loader.cpp src/utils/csv_loader.cpp)
# target_include_directories(test_csv_loader PRIVATE include include/utils)
# add_test(NAME ParticipantTest COMMAND test_participant)
# add_test(NAME AdminTest COMMAND test_admin)
# add_test(NAME CSVLoaderTest COMMAND test_csv_loader)