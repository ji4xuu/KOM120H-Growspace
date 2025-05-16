cmake_minimum_required(VERSION 3.10)
project(Growspace VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Header directories
include_directories(include)
include_directories(include/menus)
include_directories(include/services)
include_directories(include/utils)

# Source files
file(GLOB_RECURSE SOURCES
    src/*.cpp
    main.cpp
)

# Add executable
add_executable(kuda ${SOURCES})

# Untuk test (opsional - hanya jika kamu ingin build unit test juga)
# enable_testing()

# Tambahkan unit test
# add_executable(test_participant tests/test_participant.cpp src/services/participant_service.cpp src/utils/csv_loader.cpp)
# target_include_directories(test_participant PRIVATE include include/services include/utils)

# add_executable(test_admin tests/test_admin.cpp src/services/admin_service.cpp src/utils/csv_loader.cpp)
# target_include_directories(test_admin PRIVATE include include/services include/utils)

# add_executable(test_csv_loader tests/test_csv_loader.cpp src/utils/csv_loader.cpp)
# target_include_directories(test_csv_loader PRIVATE include include/utils)

# Tambahkan test target (jika kamu pakai ctest)
# add_test(NAME ParticipantTest COMMAND test_participant)
# add_test(NAME AdminTest COMMAND test_admin)
# add_test(NAME CSVLoaderTest COMMAND test_csv_loader)