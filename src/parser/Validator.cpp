#include "Validator.h"

Validator::Validator(){};

bool Validator::IsValidTimeFormat(const string& time) {
    static const regex timeRegex(R"(^\d{2}:\d{2}$)");
    if (!regex_match(time, timeRegex)) {
        return false;
    }

    int hours = std::stoi(time.substr(0, 2));
    int minutes = std::stoi(time.substr(3, 2));

    return (0 <= hours && hours <= 23) && (0 <= minutes && minutes <= 59);
}

bool Validator::IsValidId(int id){
    set <int> ids = {1,2,3,4,11,12,13};
    if(ids.find(id) != ids.end()){
        return true;
    }
    return false;
}

bool Validator::isValidName(const std::string& name) {
    for (char c : name) {
        if (!(std::islower(c) || std::isdigit(c) || c == '_' || c == '-')) {
            return false;
        }
    }
    return !name.empty(); 
}

bool Validator::IsValidNumTables(const optional<int>& num){
    const Config& config = ConfigLoader::getConfig();
    if (num == nullopt || (num > 0 && num <= config.tables)){
        return true;
    }
    return false;
}

bool Validator::IsValidTableCount(const std::string& line) {
    // Целое положительное число
    std::regex pattern(R"(^\d+$)");
    if (!std::regex_match(line, pattern)) return false;
    int num = std::stoi(line);
    return num > 0;
}

bool Validator::IsValidWorkingHours(const std::string& line) {
    // Пример: 10:00 22:00
    std::regex pattern(R"(^\d{2}:\d{2} \d{2}:\d{2}$)");
    if (!std::regex_match(line, pattern)) return false;

    std::istringstream iss(line);
    std::string openTime, closeTime;
    iss >> openTime >> closeTime;

    return IsValidTimeFormat(openTime) && IsValidTimeFormat(closeTime);
}

bool Validator::IsValidHourlyRate(const std::string& line) {
    // Целое положительное число
    std::regex pattern(R"(^\d+$)");
    if (!std::regex_match(line, pattern)) return false;
    int rate = std::stoi(line);
    return rate > 0;
}

