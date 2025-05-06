#include "ConfigLoader.h"
#include "../parser/Validator.h"
#include <iostream> // Для std::cerr
#include <cstdlib>  // Для exit()

Config ConfigLoader::config;
bool ConfigLoader::loaded = false;
bool ConfigLoader::load_failed = false; 

ConfigLoader::ConfigLoader(const string& filename) : filename(filename) {}

string ConfigLoader::StrFormatTime(int totalMinutes) {
    int hours = totalMinutes / 60;
    int minutes = totalMinutes % 60;
    char buffer[6]; // "HH:MM" + '\0'
    snprintf(buffer, sizeof(buffer), "%02d:%02d", hours, minutes);
    return string(buffer);
}


int ConfigLoader::TimeToInt(const string& time) {
    int hours = std::stoi(time.substr(0, 2));
    int minutes = std::stoi(time.substr(3, 2));
    return hours * 60 + minutes;
}

void ConfigLoader::base_info() {
    if (loaded) {
        std::cout << "Конфигурация уже загружена!" << std::endl;
        return;
    }

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла: " << filename << std::endl;
        exit(1); 
    }

    
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    if (fileSize == 0) {
        std::cout << "Файл конфигурации пуст" << std::endl;
        file.close();
        exit(1); 
        return;
    }

    std::string line;

    try {
        std::getline(file, line);
        if (Validator::IsValidTableCount(line)) {
            config.tables = std::stoi(line);
        } else {
            std::cerr << "Ошибка: некорректный формат количества столов: " << line << std::endl;
            file.close();
            throw std::runtime_error("Ошибка: некорректный формат количества столов");
            return;
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Ошибка при преобразовании количества столов: " << e.what() << std::endl;
        file.close();
        exit(1); 
        return;
    }

    try {
        std::getline(file, line);
        if (Validator::IsValidWorkingHours(line)) {
            std::istringstream iss(line);
            std::string open_str, fin_str;
            iss >> open_str >> fin_str;
            cout << open_str << endl; 
            config.st_time = TimeToInt(open_str);
            config.fin_time = TimeToInt(fin_str);
        } else {
            std::cerr << "Ошибка: некорректный формат времени: " << line << std::endl;
            file.close();
            std::runtime_error("Ошибка: некорректный формат времени");
            return;
        }
        
    } catch (const std::invalid_argument& e) {
        std::cerr << "Ошибка при преобразовании времени: " << e.what() << std::endl;
        file.close();
        exit(1); 
        return;
    }

    try {
        std::getline(file, line);
        if (Validator::IsValidHourlyRate(line)) {
            config.coast = std::stoi(line);
        } else {
            std::cerr << "Ошибка: некорректный формат стоимости: " << line << std::endl;
            file.close();
            std::runtime_error("Ошибка: некорректный формат стоимости");
            return;
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Ошибка при чтении стоимости: " << e.what() << std::endl;
        file.close();
        exit(1); 
        return;
    }

    file.close();
    loaded = true;
}

const Config& ConfigLoader::getConfig() {
    if (!loaded) {
        std::cerr << "Ошибка: конфигурация не была успешно загружена." << std::endl;
        return config;
    }
    return config;
}