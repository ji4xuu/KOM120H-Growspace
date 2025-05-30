# Growspace: Workshop and Training Management CLI Application

Growspace is a command-line interface (CLI) application designed to manage event registrations and workshop/training sessions. It supports two primary user roles: Participants and Administrators, each with a tailored set of features.

This project is a fulfillment of the Final Project for the Data Structures course.

## Key Features

**For Participants:**
* View a list of available events (both active and completed).
* Filter and sort events based on keywords, title, date, etc.
* View complete details of any event.
* Register for events that are currently active and have available quota.
* Check personal registration status (including payment status for paid events).
* Cancel a registration (if permitted by the event or system rules).

**For Administrators:**
* Secure login using a hashed password.
* **Registration Verification Management:**
    * View a queue of registrants awaiting verification (especially for paid events).
    * Process registrants (approve or reject).
* **Event Management (CRUD - Create, Read, Delete):**
    * Create new events with comprehensive details.
    * View a list of all events (including active, canceled, or finished) with detailed information.
    * Delete events (with protection: cannot be deleted if participants are already registered).
    * *(The "Update Event" feature is currently omitted to focus on core project completion).*
* **Global Undo Feature:** Cancel the last administrative action performed (e.g., undo verification, undo event creation, undo event deletion) with an interactive confirmation prompt.

## Data Structures Implemented

This application implements several core data structures:
* **Custom Queue:** Used to manage the queue of registrants awaiting payment/verification. Implemented using a linked list for efficient $O(1)$ `enqueue`, `dequeue`, and a special `enqueueFront` operation (for the undo feature).
* **Custom Stack:** Used to support the "Undo" functionality for administrator actions. Implemented using `std::vector` as its base, providing efficient $O(1)$ `push` and `pop` operations.
* **Hashing (Hash Table):** Utilizes `std::unordered_map` to create fast lookup indexes for event and registration data by ID. This enables very fast data retrieval ($O(1)$ on average) within `ParticipantService` and `AdminService`. Hash functions are also used to secure user and admin passwords.
* **Sorting:** Employs `std::sort` with custom comparators (lambda functions) to allow participants and admins to sort event lists by various criteria (ID, title, date, etc.) with a complexity of $O(N \log N)$.

## Prerequisites

Before building and running Growspace, ensure you have the following installed:
* **CMake** (version 3.10 or newer)
* **C++ Compiler** supporting C++17 (e.g., GCC, Clang, MSVC)
* **MinGW** (if on Windows using GCC) or **Make** (on Linux/macOS)
* **Git** (optional, if cloning from a repository)

## Getting Started & Building the Project

1.  **Clone the Repository (if applicable):**
    ```bash
    git clone [https://github.com/](https://github.com/)[YOUR_USERNAME]/[YOUR_REPOSITORY_NAME].git
    cd [YOUR_REPOSITORY_NAME]
    ```

2.  **Prepare Data Folder:**
    Ensure you have a `data/` directory in the root of your project containing the following CSV files:
    * `data/events.csv`
    * `data/registrations.csv`
    * `data/users.csv` (contains admin login data with hashed passwords)

3.  **Build Process with CMake:**

    * **Automated with a Script (Example for Linux/macOS):**
        If you provide a build script (e.g., `setup.sh` or `build.bat` for Windows).
        Example `setup.sh`:
        ```bash
        #!/bin/bash
        echo "Creating build directory..."
        mkdir -p build
        cd build
        echo "Running CMake..."
        cmake ..
        echo "Building project..."
        make
        echo "Build finished. Executable is in the build directory."
        ```
        Make it executable: `chmod +x setup.sh` and run `./setup.sh`.

    * **Manual Build Steps:**
        ```bash
        mkdir -p build  # Create build directory if it doesn't exist
        cd build
        ```
        **For Windows with MinGW:**
        ```bash
        cmake -G "MinGW Makefiles" ..
        cmake --build .
        ```
        **For Linux/macOS:**
        ```bash
        cmake ..
        make
        # Alternatively, after 'cmake ..':
        # cmake --build .
        ```
        The executable `growspace` (or `growspace.exe`) will be generated in the `build/` directory.

## Running the Application

After a successful build, run the application from within the `build/` directory:

```bash
./growspace      # For Linux/macOS
# or
.\growspace.exe  # For Windows (PowerShell)
# or
growspace.exe    # For Windows (Command Prompt)

## Usage

From the `build/` directory:

```bash
./growspace       # Linux/macOS
growspace.exe     # Windows (MinGW)

Note on Data Files: The application, when run from the build directory, will typically look for the data folder in the parent directory (i.e., ../data/). If you encounter issues with file loading, ensure your CSV reading functions in csv_loader.cpp use paths relative to the executable's expected working directory, or copy the data folder into your build directory. A common approach is ../data/filename.csv.

The application will display the main menu where users can choose to proceed as a Participant or log in as an Administrator.
 
```

## Project Structure

```
KOM120H-Growspace/
├── CMakeLists.txt              # CMake build configuration
├── README.md                   # Project information (this file)
├── LICENSE                     # Project license file (e.g., MIT)
├── data/                       # Directory for CSV data files
│   ├── events.csv
│   ├── registrations.csv
│   └── users.csv
├── include/                    # Directory for all header files (.h)
│   ├── menus/                  # Headers for menu interface logic
│   │   ├── admin_menu.h
│   │   ├── main_menu.h
│   │   └── participant_menu.h
│   ├── services/               # Headers for application business logic
│   │   ├── admin_service.h
│   │   ├── auth_service.h
│   │   └── participant_service.h
│   └── utils/                  # Headers for common utilities and data models
│       ├── csv_loader.h
│       ├── hashing.h
│       ├── models.h            # Struct and enum definitions
│       ├── other.h
│       ├── queue.h             # Custom Queue implementation
│       └── stack.h             # Custom Stack implementation
├── src/                        # Directory for all implementation files (.cpp)
│   ├── menus/                  # Menu interface implementations
│   │   ├── admin_menu.cpp
│   │   ├── main_menu.cpp
│   │   └── participant_menu.cpp
│   ├── services/               # Business logic implementations
│   │   ├── admin_service.cpp
│   │   ├── auth_service.cpp
│   │   └── participant_service.cpp
│   └── utils/                  # Utility implementations
│       ├── csv_loader.cpp
│       ├── hashing.cpp
│       └── other.cpp
├── main.cpp                    # Main entry point of the application
└── build/                      # (Generated by CMake, contains executable & build files)
```

---

## License

This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details.