#ifndef OTHER_H
#define OTHER_H



#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <regex>


void clearScreen();
std::string getCurrentDate();
std::string read_line();
void pause();
bool isValidDateFormat(const std::string& date_str);

#endif

