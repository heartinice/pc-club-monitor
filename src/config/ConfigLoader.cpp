#include "ConfigLoader.h"
#include "../parser/Validator.h"

Config ConfigLoader::config; //обдумать делать ли статические переменные 
bool ConfigLoader::loaded = false;

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
        return;
    }

    // Проверяем размер файла
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    if (fileSize == 0) {
        std::cout << "Файл конфигурации пуст" << std::endl;
        return;
    }

    std::string line;

    try {
        std::getline(file, line);
        if(Validator::IsValidTableCount(line))
        config.tables = std::stoi(line);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Ошибка при чтении количества столов: " << e.what() << std::endl;
        return;
    }

    try {
        std::getline(file, line);
        std::istringstream iss(line);
        std::string open_str, fin_str;
        if (!(iss >> open_str >> fin_str)) {
            std::cerr << "Ошибка при чтении времени открытия/закрытия: некорректный формат строки" << std::endl;
            return;
        }
        if(Validator::IsValidWorkingHours(line)){
            config.st_time = TimeToInt(open_str);
            config.fin_time = TimeToInt(fin_str);
        }
        cout << open_str << endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Ошибка при преобразовании времени: " << e.what() << std::endl;
        return;
    }

    try {
        std::getline(file, line);
        if(Validator::IsValidHourlyRate(line))
        config.coast = std::stoi(line);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Ошибка при чтении стоимости: " << e.what() << std::endl;
        return;
    }
    
    loaded = true; 
}

const Config& ConfigLoader::getConfig() {
    if (!loaded) {
        std::cerr << "Ошибка: конфигурация не была загружена!" << std::endl;
        throw std::runtime_error("Конфигурация не была загружена!");
    }
    return config; 
}
