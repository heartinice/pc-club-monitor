#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <regex>
#include <set>
#include "../config/Config.h"
#include "../config/ConfigLoader.h"
#include <optional>
using namespace std;
class Validator{
public:
    Validator();
    static bool IsValidTimeFormat(const std::string& time);
    static bool IsValidId(const int id);
    static bool isValidName(const std::string& name);
    static bool IsValidNumTables(const optional<int>& num);
    static bool IsValidTableCount(const std::string& line);
    static bool IsValidWorkingHours(const std::string& line);
    static bool IsValidHourlyRate(const std::string& line);
};

#endif // VALIDATOR_H


