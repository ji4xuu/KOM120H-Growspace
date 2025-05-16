#ifndef CSV_LOADER_H
#define CSV_LOADER_H

#include "models.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

// Function declarations
std::vector<User> readUserFromCSV(const std::string& filename);
std::vector<Event> readEventFromCSV(const std::string& filename);
std::vector<Registration> readRegistrationFromCSV(const std::string& filename);
bool appendUser(const User& user, const std::string& filename = "data/users.csv");
bool appendEvent(const Event& event, const std::string& filename = "data/events.csv");
bool appendRegistration(const Registration& registration, const std::string& filename = "data/registrations.csv");
void writeUserToCSV(const std::vector<User>& users, const std::string& filename);
void writeEventToCSV(const std::vector<Event>& events, const std::string& filename);
void writeRegistrationToCSV(const std::vector<Registration>& registrations, const std::string& filename);

#endif // CSV_LOADER_H