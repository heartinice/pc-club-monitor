// Validator.h
#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
#include <regex>
#include <optional>
#include <cctype>
#include <set>

class Validator {
public:
    Validator();
    static bool IsValidTimeFormat(const std::string& time);
    static bool IsValidId(int id); 
    static bool isValidName(const std::string& name);
    static bool IsValidNumTables(const std::optional<int>& num); 
    static bool IsValidTableCount(const std::string& line);
    static bool IsValidWorkingHours(const std::string& line);
    static bool IsValidHourlyRate(const std::string& line);
    static bool IsTimeSequential(const std::string& currentTime, const std::optional<std::string>& previousTime);
    static bool IsValidTableNumber(const std::optional<int>& tableNumber);
};

#endif